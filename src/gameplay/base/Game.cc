#include "Game.hh"

#include "gameplay/objs/Slot.hh"
#include "engine/virtual/Input.hh"
#include "engine/virtual/Framework.hh"
#include "engine/virtual/Window.hh"
#include "lua/LuaSupport.hh"
#include "spdlog/spdlog.h"
using namespace spdlog;

static GameMap *activeMap = nullptr;
static World *activeWorld = nullptr;

static GameRules createDefaultGameRules()
{
    GameRules gr;
    gr.judgeValue = {
        .perfect = 300,
        .almost = 200,
        .accepted = 150,
        .medium = 100,
        .touched = 50,
        .lost = 0,
        .spaceNotesAmplifier = 2.0,
    };

    gr.judgeTime = {
        .range = 0.2,
        .good = 0.15,
        .almost = 0.08,
        .perfect = 0.04,
        .allowBreak = 0.04,
    };
    gr.inputOptn = {
        .judgeRayAngle = 0.015,
    };
    return gr;
}

static int useCam(lua_State *l)
{
    auto cid = lua_tostring(l, -1);
    if (activeMap != nullptr && activeWorld != nullptr && activeMap->namedObjects.contains(cid))
    {
        auto obj = activeMap->namedObjects[cid];
        if (obj->type == CAMERA)
        {
            activeWorld->activeCamera = (Camera *)obj->nativeObj;
        }
    }
    return 1;
}

static AbstractNote *createNote(NoteObject *obj, World *wd, ScoreManager *sm)
{
    AbstractNote *a;
    switch (obj->noteType)
    {
    case TAPU:
        a = new Tapu;
        break;
    case SZKU:
        a = new Shizuku;
        break;
    case HOSHI:
        a = new Hoshi;
        break;
    case PRSU:
        a = new Puresu;
        ((Puresu *)a)->absLength = obj->length;
        break;
    case HASHI:
        a = new Hashi;
        ((Hashi *)a)->absLength = obj->length;
        break;
    case KZTU:
    case AKU:
    case SGKI:
    case TSAR:
    default:
        warn("Creating note with missing type: " + std::to_string(obj->noteType));
        break;
    }
    a->hitTime = obj->hitTime;
    // TODO: impl fake notes
    a->controller = new ObjController(obj);
    a->world = wd;
    a->score = sm;
    obj->nativeObj = a;
    return a;
}

void Game::initGame(const std::string &mapId)
{
    // Init camera set
    // Due to code limitations, camera function has to be set here
    luaBind("__native_use_cam", useCam);
    // Load map
    info("Initializing game for '" + mapId + "'");
    info("Loading map '" + mapId + "'");
    map = loadMap(mapId);
    if (!map.verified)
    {
        error("Failed to verify map '" + mapId + "'. Is this map corrupted?");
        return;
    }
    unsigned int counter = 0;

    // Generate objects
    for (auto &o : map.objects)
    {
        ++counter;
        switch (o->type)
        {
        case NOTE:
            pendingNotes.push_back(createNote((NoteObject *)o, &world, &score));
            break;
        case SLOT:
        {
            auto s = new Slot;
            s->controller = new ObjController(o);
            s->variant = ((SlotObject *)o)->slotType;
            pendingSlots.push_back(s);
            o->nativeObj = s;
        }
        break;
        case CAMERA:
        {
            auto cam = new Camera;
            cam->controller = new ObjController(o);
            cameras.insert(cam);
            o->nativeObj = cam;
        }
        break;
        default:
            --counter;
            warn("Unrecognized object type detected: " + std::to_string(o->type));
            break;
        }
    }

    // Link controllers
    for (auto &o : map.objects)
    {
        if (o->relTarget != nullptr && o->nativeObj != nullptr)
        {
            ((TickObject *)o->nativeObj)->controller->setDep(((TickObject *)o->relTarget->nativeObj)->controller);
        }
    }
    // Sort objects
    pendingNotes.sort();
    pendingSlots.sort();
    info("Map generation complete. " + std::to_string(counter) + " objects generated.");

    // Init gameplay
    activeMap = &map;
    activeWorld = &world;
    int w, h;
    vtGetWindowSize(w, h);
    world.screenSize[0] = w;
    world.screenSize[1] = h;
    input.setupInputListeners();
    mapTimer = Timer(vtGetTime);
    absTimer = Timer(vtGetTime);
    score.rules = createDefaultGameRules();
    return;
}

void Game::runOnce()
{
    DrawContext dCtx;
    auto mapTimeNow = mapTimer.getTime();

    // Process timers
    mapSchedule.notify(mapTimeNow);
    absSchedule.notify(absTimer.getTime());

    // Handle events
    input.pollInputEvents();

    // Load and unload objects
    // Cameras are always loaded, so they won't be ticked
    for (auto it = pendingNotes.begin(); it != pendingNotes.end();) // Do not it++
    {
        auto n = *it;
        if (n->controller->shouldLoad(mapTimeNow))
        {
            activeNotes.insert(n);
            it = pendingNotes.erase(it);
        }
        else
        {
            break; // Notes are sorted
        }
    }
    for (auto it = pendingSlots.begin(); it != pendingSlots.end();)
    {
        auto n = *it;
        if (n->controller->shouldLoad(mapTimeNow))
        {
            activeSlots.insert(n);
            it = pendingSlots.erase(it);
        }
        else
        {
            break;
        }
    }

    for (auto it = activeNotes.begin(); it != activeNotes.end();)
    {
        auto n = *it;
        if (n->controller->shouldUnload(mapTimeNow))
        {
            it = activeNotes.erase(it);
            doneNotes.insert(n);
        }
        else
        {
            ++it;
        }
    }

    for (auto it = activeSlots.begin(); it != activeSlots.end();)
    {
        auto n = *it;
        if (n->controller->shouldUnload(mapTimeNow))
        {
            it = activeSlots.erase(it);
            doneSlots.insert(n);
        }
        else
        {
            ++it;
        }
    }

    // Tick objects
    // Slot first
    for (auto &s : activeSlots)
    {
        s->tick(mapTimeNow);
    }
    // Notes
    for (auto &n : activeNotes)
    {
        n->performJudge(mapTimeNow, input, score);
        n->tick(mapTimeNow);
    }
    // Cameras
    for (auto &c : cameras)
    {
        c->tick(mapTimeNow);
    }

    // Draw objects
    for (auto &s : activeSlots)
    {
        s->draw(dCtx);
    }
    for (auto &n : activeNotes)
    {
        n->draw(dCtx);
    }
    if (world.activeCamera != nullptr)
    {
        dCtx.cam = *world.activeCamera;
    }
    vtDraw(dCtx);
    vtFinalizeDraw(dCtx);
    vtWindowLoop();
    // TODO: add game logic process here
}

void Game::runMainLoop()
{
    while (status != DONE)
    {
        runOnce();
    }
}