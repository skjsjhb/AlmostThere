#include "MapLoad.hh"

#include "lua/LuaSupport.hh"
#include "support/Resource.hh"
#include "util/Util.hh"
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <limits>
#include <cstdlib>
#include "spdlog/spdlog.h"
using namespace spdlog;

#define CHART_FILE "chart.lua"

#define META_SEC_DELM "-----\n"
#define OBJ_SEC_DELM "----\n"
#define OBJ_HEAD_BODY_DELM "---\n"

#define NOTE_SLOT_TYPE_SPLIT 10
#define TYPE_CAMERA -1
#define SUPPORT_VERSIONS \
    {                    \
        1                \
    }

static MapObject *activeObjectBuffer = nullptr;

static bool hasCircularRef(const MapObject *om)
{
    std::set<MapObject *> paths;
    MapObject *currentRel = om->relTarget;
    while (currentRel != nullptr)
    {
        paths.insert(currentRel);
        currentRel = currentRel->relTarget;
        if (paths.contains(currentRel))
        {
            return true; // Circular
        }
    }
    return false; // All checked
}

static int objectRegistry(lua_State *l)
{
    // Sig: __native_objreg(type, id, player, genTime, endTime, bindObj, length)
    double length = lua_tonumber(l, -1);
    std::string bindObj = lua_tostring(l, -2);
    double endTime = lua_tonumber(l, -3);
    double genTime = lua_tonumber(l, -4);
    if (genTime < 0)
    {
        genTime = 0;
    }
    if (endTime < 0)
    {
        endTime = std::numeric_limits<double>::max();
    }
    int player = lua_tointeger(l, -5);
    std::string id = lua_tostring(l, -6);
    int type = lua_tointeger(l, -7);
    if (type == TYPE_CAMERA)
    {
        // Create camera
        CameraObject *c = new CameraObject;
        *c = CameraObject();
        c->genTime = genTime;
        c->hitTime = endTime;
        c->player = -1;
        c->id = id;
        c->type = CAMERA;
        c->relTargetName = bindObj;
        activeObjectBuffer = c;
    }
    else if (type >= NOTE_SLOT_TYPE_SPLIT)
    {
        // Create slot
        SlotObject *m = new SlotObject;
        m->genTime = genTime;
        m->hitTime = endTime;
        m->player = player;
        m->id = id;
        m->type = SLOT;
        m->relTargetName = bindObj;
        m->slotType = (SlotVariant)(type - NOTE_SLOT_TYPE_SPLIT);
        activeObjectBuffer = m;
    }
    else
    {
        // Create note
        NoteObject *n = new NoteObject;
        n->length = length;
        n->genTime = genTime;
        n->hitTime = endTime;
        n->player = player;
        n->id = id;
        n->type = NOTE;
        n->noteType = (NoteType)type;
        n->relTargetName = bindObj; // Low-level links are not done here.
        activeObjectBuffer = n;
    }
    return 1;
}

bool verifyMap(const GameMap &map)
{
    // TODO: verify map
    return true;
}

#define UNNAMED_OBJ_PREFIX "_UN_"

GameMap loadMap(const std::string &mapId)
{
    GameMap m;
    std::ifstream chartFile(getMapResource(mapId, "chart.lua"));

    if (chartFile.fail())
    {
        error("Cannot load map '" + mapId + "'. Either the chart file is missing, or it's not readable.");
        return m;
    }

    std::stringstream ss;
    ss << chartFile.rdbuf();
    std::string content = ss.str();

    auto metaBody = splitStr(content, META_SEC_DELM, 2);
    if (metaBody.size() != 2)
    {
        error("The structure of map '" + mapId + "' is corrupted. Either meta section or body is missing.");
        return m;
    }

    // Load meta section
    auto metaSec = metaBody[0];

    MapMeta mt;
    luaRun(metaSec); // Execute meta script

    auto mapVersion = luaGetNumber("MapVersion");
    std::set<int> supportVs(SUPPORT_VERSIONS);

    if (!supportVs.contains(mapVersion))
    {
        error("Unsupported map version " + std::to_string(mapVersion));
        return m;
    }

    mt.author = luaGetStr("Author");
    mt.name = luaGetStr("Title");
    mt.id = luaGetStr("MapId");
    mt.mapper = luaGetStr("Chart");
    mt.diff = luaGetStr("DiffName");
    mt.song = luaGetStr("Song");
    mt.duration = luaGetNumber("Duration");

    m.meta = mt;

    // Load objects

    auto objSrcs = splitStr(metaBody[1], OBJ_SEC_DELM);

    int unnamedCounter = 0;
    for (auto &os : objSrcs)
    {
        auto objSecs = splitStr(os, OBJ_HEAD_BODY_DELM, 2);

        if (objSecs.size() != 2)
        {
            // Invalid
            warn("Incomplete object detected. Missing either head or body.");
            continue;
        }

        // Init section
        activeObjectBuffer = nullptr;
        luaRun(objSecs[0]); // Register

        if (activeObjectBuffer == nullptr)
        {
            // Not registered, no mem allocated
            warn("Object without registry detected. An object won't run without registering. Skipped.");
            continue;
        }
        auto pm = activeObjectBuffer;
        pm->tickScript = luaPrecompile(objSecs[1]);
        if (pm->tickScript == LUA_NOREF)
        {
            warn("Invalid object ticking script detected. Removed.");
            free(pm);
            continue;
        }
        if (pm->id != "")
        {
            m.namedObjects[pm->id] = pm;
        }
        else
        {
            // Assign a random one and do not put it into named objects
            pm->id += UNNAMED_OBJ_PREFIX + std::to_string(unnamedCounter);
            unnamedCounter++;
        }
        m.objects.push_back(pm);
    }

    // Link objects
    for (auto &o : m.objects)
    {
        if (o->relTargetName != "")
        {
            if (m.namedObjects.contains(o->relTargetName))
            {
                o->relTarget = m.namedObjects[o->relTargetName];
            }
        }
    }

    // Check for circulars
    for (auto &o : m.objects)
    {
        if (o->relTarget != nullptr)
        {
            if (hasCircularRef(o))
            {
                warn("Circular reference detected and blocked for object '" + o->id + "'. Check your map.");
                o->relTarget = nullptr;
            }
        }
    }
    m.verified = verifyMap(m);
    return m;
}

void freeMap(GameMap &m)
{
    for (auto p : m.objects)
    {
        free(p);
    }
}

void initMapLoader()
{
    luaBind("__native_objreg", objectRegistry);
}