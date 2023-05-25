#include "Controller.hh"

#include "gameplay/map/MapDef.hh"
#include "lua/LuaSupport.hh"
#include <cglm/cglm.h>
#include <list>
#include <set>
#include <unordered_map>
#include <any>
#include <typeinfo>

#define IDF_RELMODE "RELMODE"
#define IDF_POS "POS"
#define IDF_UP "UP"
#define IDF_NORMAL "NORMAL"
#define IDF_PCT "PCT"
#define IDF_TIME "TIME"
#define IDF_FOV "FOV"
#define IDF_OBJID "OBJID"
#define IDF_ALPHA "ALPHA"

ObjState ObjController::getState() const
{
    return currentState;
}

const ObjMinimalRef &ObjController::getReference() const
{
    return objRef;
}

enum InVarType
{
    NUMBER,
    NIL,
    STRING,
    BOOL,
    TABLE // Out only
};

static std::unordered_map<std::string, std::pair<InVarType, std::any>> inValuePool, outValuePool;

static int incImpl(lua_State *l)
{
    auto cid = std::string(lua_tostring(l, -1));
    if (inValuePool.contains(cid))
    {
        auto v = inValuePool[cid];
        switch (v.first)
        {
        case NUMBER:
            lua_pushnumber(l, std::any_cast<double>(v.second));
            return 1;
        case STRING:
            lua_pushstring(l, std::any_cast<std::string>(v.second).c_str());
            return 1;
        case BOOL:
            lua_pushboolean(l, std::any_cast<bool>(v.second));
            return 1;
        case NIL:
        default:
            break;
        }
    }
    lua_pushnil(l);
    return 1;
}

static int outImpl(lua_State *l)
{
    auto cid = std::string(lua_tostring(l, -2));
    switch (lua_type(l, -1))
    {
    case LUA_TNUMBER:
        outValuePool[cid] = std::pair(NUMBER, lua_tonumber(l, -1));
        return 1;
    case LUA_TBOOLEAN:
        outValuePool[cid] = std::pair(BOOL, lua_toboolean(l, -1));
        return 1;
    case LUA_TSTRING:
        outValuePool[cid] = std::pair(STRING, std::string(lua_tostring(l, -1)));
        return 1;
    case LUA_TTABLE:
        outValuePool[cid] = std::pair(TABLE, luaBuildTable(-1));
        return 1;
    default:
        // No-op
        return 1;
    }
    return 1;
}

static void readVec3(const std::string &outName, vec3 d)
{
    if (!outValuePool.contains(outName))
    {
        return;
    }
    auto t = outValuePool[outName];
    if (t.first == TABLE)
    {
        for (int i = 0; i < 3; i++)
        {
            d[i] = std::stod(std::any_cast<StringTable>(t.second)[std::to_string(i + 1)]);
        }
    }
}

static double readNumber(const std::string &outName, double dv)
{
    if (!outValuePool.contains(outName))
    {
        return dv;
    }
    double p = dv;
    auto iva = outValuePool[outName];
    if (iva.first == NUMBER)
    {
        p = std::any_cast<double>(iva.second);
    }
    return p;
}

// TODO: readStr, readBool

static void relVec(vec3 org, vec3 base, vec3 er, vec3 eu, vec3 en, PositionMethod md)
{
    vec3 p;
    switch (md)
    {
    case REPROJ:
        for (int i = 0; i < 3; i++)
        {
            p[i] = org[0] * er[i] + org[1] * eu[i] + org[2] * en[i];
        }
        break;
    case PERSPECTIVE:
        for (int i = 0; i < 3; i++)
        {
            p[i] = base[i] + org[0] * er[i] + org[1] * eu[i] + org[2] * en[i];
        }
        break;
    case OFFSET:
        for (int i = 0; i < 3; i++)
        {
            p[i] = base[i] + org[i];
        }
        break;
    case ABSOLUTE:
    default:
        return;
    }
    glm_vec3_copy(p, org);
}

void ObjController::tick(double absTime)
{
    if (lastTickTime == absTime)
    {
        // Up to date
        return;
    }

    if (!rel.expired())
    {
        // Tick deps first
        rel.lock()->tick(absTime);
    }
    // Sync time flag
    lastTickTime = absTime;

    // Calculate the length value first
    // Note: length value is calculated by the system and cannot be modified by the script.
    // This is worked as intended.
    if (objRef.length != 0)
    {
        auto unloadTime = objRef.endTime + objRef.length;
        if (absTime <= objRef.endTime)
        {
            currentState.len = objRef.length;
        }
        else if (absTime < unloadTime && absTime > objRef.endTime)
        {
            currentState.len = objRef.length - (absTime - objRef.endTime);
        }
        else
        {
            currentState.len = 0;
        }
    }
    else
    {
        currentState.len = 0;
    }

    // Calculate related variable
    double time = objRef.endTime - objRef.genTime;
    double pct;
    if (time == 0)
    {
        pct = 1;
    }
    else
    {
        pct = (absTime - objRef.genTime) / time;
    }

    if (pct > 1)
        pct = 1;
    if (pct < 0)
        pct = 0;
    if (time < 0)
        time = 0;

    // Setup variables
    inValuePool[IDF_PCT] = std::pair(NUMBER, std::any(pct));
    inValuePool[IDF_TIME] = std::pair(NUMBER, std::any(time));
    inValuePool[IDF_OBJID] = std::pair(STRING, std::any(objRef.id));

    // Eval script
    luaRunCompiledCode(objRef.tickScript);

    // Clean input
    inValuePool.clear();

    if (outValuePool.size() == 0)
    {
        // No updates, skipped
        return;
    }

    // Extract values
    // This is not the final coord, needs to be inherited
    vec3 iPos = {0}, iUp = {0}, iNorm = {0}, tMode = {0};
    int mode[3];
    readVec3(IDF_POS, iPos);
    readVec3(IDF_UP, iUp);
    readVec3(IDF_NORMAL, iNorm);
    readVec3(IDF_RELMODE, tMode);
    for (int i = 0; i < 3; i++)
    {
        mode[i] = (int)tMode[i];
    }
    currentState.alpha = readNumber(IDF_ALPHA, 1);

    if (!rel.expired())
    {
        float *tPos, *tUp, *tNorm;
        vec3 tRight;
        auto dep = rel.lock();
        tUp = dep->currentState.up;
        tNorm = dep->currentState.normal;
        tPos = dep->currentState.pos;
        glm_vec3_cross(tUp, tNorm, tRight);
        glm_vec3_normalize(tRight);

        // Update relative position method
        relVec(iPos, tPos, tRight, tUp, tNorm, (PositionMethod)mode[0]);
        relVec(iUp, tPos, tRight, tUp, tNorm, (PositionMethod)mode[1]);
        relVec(iNorm, tPos, tRight, tUp, tNorm, (PositionMethod)mode[2]);
    }

    glm_vec3_copy(iPos, currentState.pos);
    glm_vec3_copy(iUp, currentState.up);
    glm_vec3_copy(iNorm, currentState.normal);

    // Cleanup output
    outValuePool.clear();
}

ObjMinimalRef::ObjMinimalRef(const MapObject &m)
{
    id = m.id;
    tickScript = m.tickScript;
    player = m.player;
    genTime = m.genTime;
    endTime = m.endTime;
    if (m.type == NOTE)
    {
        length = dynamic_cast<NoteObject &>(const_cast<MapObject &>(m)).length;
    }
}

void initControllerLuaExt()
{
    luaBind("__native_inc", incImpl);
    luaBind("__native_out", outImpl);
}
