#include "Game.hh"

#include "gameplay/objs/Slot.hh"
#include "gameplay/objs/Note.hh"
#include "gameplay/map/MapLoad.hh"
#include "engine/virtual/Input.hh"
#include "engine/virtual/Framework.hh"
#include "engine/virtual/Window.hh"
#include "engine/virtual/Graphics.hh"
#include "gameplay/player/Player.hh"
#include "user/Account.hh"
#include "lua/LuaSupport.hh"
#include "spdlog/spdlog.h"
#include "support/Resource.hh"
#include <set>
using namespace spdlog;

static View *activeView = nullptr;
static Game *activeGame = nullptr;

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

// Active camera interface
static int useCam(lua_State *l)
{
    auto cid = lua_tostring(l, -1);
    if (activeGame != nullptr && activeGame->objects.namedObjects.contains(cid))
    {
        auto obj = std::dynamic_pointer_cast<Camera>(activeGame->objects.namedObjects[cid].lock());
        if (obj)
        {
            activeView->camera = obj;
        }
    }
    return 1;
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

    if (map.meta.song.size() > 0)
    {
        audio.bgmBuf = vtLoadAudio(getMapResource(mapId, map.meta.song));
        if (audio.bgmBuf == 0)
        {
            warn("Missing bgm audio buffer, bgm won't be played.");
        }
    }

    std::set<std::pair<std::string, std::string>> linkRelation;

    unsigned int counter = 0;

    // Generate objects
    for (auto &o : map.objects)
    {
        if (o->relTargetName.size() > 0 && o->id.size() > 0)
        {
            linkRelation.insert({o->id, o->relTargetName});
        }
        ++counter;
        std::shared_ptr<TickObject> tck;
        switch (o->type)
        {
        case NOTE:
            tck = Note::createNote(std::dynamic_pointer_cast<NoteObject>(o), *this);
            break;
        case SLOT:
            tck = Slot::createSlot(std::dynamic_pointer_cast<SlotObject>(o));
            break;
        case CAMERA:
            tck = Camera::createCamera(std::dynamic_pointer_cast<CameraObject>(o));
            break;
        default:
            --counter;
            warn("Unrecognized object type detected: " + std::to_string(o->type));
            break;
        }
        if (tck)
        {
            objects.bufferedObjects.push_back(tck);
            if (o->id.size() > 0)
            {
                objects.namedObjects[o->id] = tck;
            }
        }
    }

    for (auto &r : linkRelation)
    {
        auto base = objects.namedObjects[r.first];
        auto target = objects.namedObjects[r.second];
        if (!base.expired() && !target.expired())
        {
            base.lock()->getController().lock()->setRel(target.lock()->getController());
        }
    }

    // Sort objects
    objects.bufferedObjects.sort([](const std::shared_ptr<TickObject> &a, const std::shared_ptr<TickObject> &b) -> bool
                                 { return a->getTickTime() < b->getTickTime(); });
    info("Map generation complete. " + std::to_string(counter) + " objects generated.");

    // Init gameplay

    // Bind globals for `useCam`
    activeView = &view;
    activeGame = this;

    int w, h;
    vtGetWindowSize(w, h);
    view.screenSize[0] = w;
    view.screenSize[1] = h;
    input.setupInputListeners();
    mapTimer = Timer(vtGetTime);
    absTimer = Timer(vtGetTime);
    score.rules = createDefaultGameRules();
    return;
}

void Game::addPlayer(const Account &account, CharID selectedChar)
{
    auto pid = players.size() + 1;
    auto xp = Player::createPlayer(selectedChar, account.getUserName(), account.getUID(), pid, account.getAccountType() == AC_REMOTE);
    players.push_back(xp);
}

void Game::runOnce()
{
    if (status == RUNNING && !audio.bgmPlaying && audio.bgmBuf != 0)
    {
        vtPlayAudio(audio.bgmBuf);
    }
    else if (audio.bgmPlaying && audio.bgmBuf != 0)
    {
        vtPauseAudio(audio.bgmBuf);
    }

    auto mapTimeNow = mapTimer.getTime();

    if (mapTimeNow > map.meta.duration)
    {
        status = DONE;
        return;
    }

    // Handle events
    input.pollInputEvents();

    // Load and unload objects
    for (auto it = objects.bufferedObjects.begin(); it != objects.bufferedObjects.end();)
    {
        auto n = *it;
        if (n->shouldTick(mapTimeNow))
        {
            objects.activeObjects.insert(n);
            it = objects.bufferedObjects.erase(it);
            continue;
        }
        // If an object is not loaded, the followings won't either.
        break;
    }

    for (auto it = objects.activeObjects.begin(); it != objects.activeObjects.end();)
    {
        auto n = *it;
        // If an object shouldn't be ticked, it has gone out of scope.
        if (!n->shouldTick(mapTimeNow))
        {
            it = objects.activeObjects.erase(it);
            // It might be necessary to collect them in the future, but so far not.
        }
        else
        {
            ++it;
        }
    }

    // Tick objects
    for (auto &s : objects.activeObjects)
    {
        // Judgement of notes have been integrated into tick method, no need to run them again.
        s->tick(mapTimeNow);
    }

    // Draw objects
    for (auto &s : objects.activeObjects)
    {
        s->draw(drawContext);
    }

    if (!view.camera.expired())
    {
        drawContext.cam = view.camera.lock();
    }

    vtProcessMeshes(drawContext);
    vtCompleteDraw(drawContext);

    // Context cleanup and reuse
    drawContext.polygons.clear();
    drawContext.shapes.clear();
    drawContext.typos.clear();

    // Close signals are ignored during gameplay
    vtWindowLoop();
}

void Game::runMainLoop()
{
    while (status != DONE)
    {
        runOnce();
    }
}