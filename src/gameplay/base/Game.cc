#include "Game.hh"

#include "gameplay/objs/Slot.hh"
#include "gameplay/objs/Note.hh"
#include "gameplay/map/MapLoad.hh"
#include "engine/virtual/Framework.hh"
#include "engine/virtual/Window.hh"
#include "gameplay/player/Player.hh"
#include "user/Account.hh"
#include <glm/gtc/matrix_transform.hpp>
#include "lua/LuaSupport.hh"
#include "spdlog/spdlog.h"
#include "support/Resource.hh"
#include <set>

using namespace spdlog;

static View *activeView = nullptr;
static Game *activeGame = nullptr;

// Active camera interface
static int useCam(lua_State *l) {
    auto cid = lua_tostring(l, -1);
    if (activeGame != nullptr && activeGame->objects.namedObjects.contains(cid)) {
        auto obj = std::dynamic_pointer_cast<Camera>(activeGame->objects.namedObjects[cid].lock());
        if (obj) {
            activeView->camera = obj;
        }
    }
    return 1;
}

void Game::initGame(const std::string &mapId) {
    // Init camera set
    // Due to code limitations, camera function has to be set here
    luaBind("__native_use_cam", useCam);
    // Load map
    info("Initializing game for '" + mapId + "'");
    info("Loading map '" + mapId + "'");
    map = loadMap(mapId);
    if (!map.verified) {
        error("Failed to verify map '" + mapId + "'. Is this map corrupted?");
        return;
    }

    if (!map.meta.song.empty()) {
        audio.bgmBuf = vtLoadAudio(getMapResource(mapId, map.meta.song));
        if (audio.bgmBuf == 0) {
            warn("Missing bgm audio buffer, bgm won't be played.");
        }
    }

    std::set<std::pair<std::string, std::string>> linkRelation;

    unsigned int counter = 0;

    // Generate objects
    for (auto &o: map.objects) {
        if (!o->relTargetName.empty() && !o->id.empty()) {
            linkRelation.insert({o->id, o->relTargetName});
        }
        ++counter;
        std::shared_ptr<TickObject> tck;
        switch (o->type) {
            case NOTE:
                tck = Note::createNote(std::dynamic_pointer_cast<NoteObject>(o), *this);
                break;
            case SLOT:
                tck = Slot::createSlot(std::dynamic_pointer_cast<SlotObject>(o), *this);
                break;
            case CAMERA:
                tck = Camera::createCamera(std::dynamic_pointer_cast<CameraObject>(o), *this);
                break;
            default:
                --counter;
                warn("Unrecognized object type detected: " + std::to_string(o->type));
                break;
        }
        if (tck) {
            objects.bufferedObjects.push_back(tck);
            if (!o->id.empty()) {
                objects.namedObjects[o->id] = tck;
            }
        }
    }

    for (auto &r: linkRelation) {
        auto base = objects.namedObjects[r.first];
        auto target = objects.namedObjects[r.second];
        if (!base.expired() && !target.expired()) {
            base.lock()->getController().lock()->setRel(target.lock()->getController());
        }
    }

    // Sort objects
    objects.bufferedObjects.sort(
            [](const std::shared_ptr<TickObject> &a, const std::shared_ptr<TickObject> &b) -> bool {
                return a->getTickTime() < b->getTickTime();
            });
    info("Map generation complete. " + std::to_string(counter) + " objects generated.");

    // Init gameplay

    // Bind globals for `useCam`
    activeView = &view;
    activeGame = this;

    int w, h;
    vtGetWindowSize(w, h);
    view.screenSize[0] = float(w);
    view.screenSize[1] = float(h);
    mapTimer = Timer(vtGetTime);
    absTimer = Timer(vtGetTime);

    // Set background
    if (!map.meta.bgimg.empty()) {
        vtSetBackground(getMapResource(mapId, map.meta.bgimg));
    } else {
        vtSetBackground("");
    }
}

void Game::addPlayer(const Account &account, CharID selectedChar) {
    auto epid = players.size() + 1;
    auto xp = Player::createPlayer(selectedChar, account.getUserName(), account.getUID(), epid,
                                   account.getAccountType() == AC_REMOTE);
    players.push_back(xp);
}

void Game::runOnce() {
    if (status == RUNNING && !audio.bgmPlaying && audio.bgmBuf != 0) {
        vtPlayAudio(audio.bgmBuf);
    } else if (audio.bgmPlaying && audio.bgmBuf != 0) {
        vtPauseAudio(audio.bgmBuf);
    }

    mapTimer.update();
    auto mapTimeNow = mapTimer.getTime();

    if (mapTimeNow > map.meta.duration) {
        status = DONE;
        return;
    } // TODO: end when squad eliminated

    // Handle events
    inputBuf = vtGetInputBuffer();

    // Load and unload objects
    for (auto it = objects.bufferedObjects.begin(); it != objects.bufferedObjects.end();) {
        auto n = *it;
        if (n->shouldTick()) {
            objects.activeObjects.insert(n);
            it = objects.bufferedObjects.erase(it);
            continue;
        }
        // If an object is not loaded, the followings won't either.
        break;
    }

    for (auto it = objects.activeObjects.begin(); it != objects.activeObjects.end();) {
        auto n = *it;
        // If an object shouldn't be ticked, it has gone out of scope.
        if (!n->shouldTick()) {
            it = objects.activeObjects.erase(it);
            // TODO: It might be necessary to collect them in the future, but so far not.
        } else {
            ++it;
        }
    }

    // Tick objects
    for (auto &s: objects.activeObjects) {
        // Judgement of notes have been integrated into tick method, no need to run them again.
        s->tick();
    }

    if (vtShouldDraw()) {
        if (view.camera.expired()) {
            error("No camera set for game objects drawing. Skipped.");
        }
        // Update draw context
        ctx3D.viewMat = view.camera.lock()->getViewMatrix();
        ctx3D.projMat = view.camera.lock()->getProjectionMatrix();

        // Update ortho projection
        ctxUI.viewMat = glm::mat4();
        ctxUI.projMat = glm::ortho(0.0f, 1600.0f, 0.0f, 900.0f); // Constant projection matrix

        // Draw objects
        for (auto &s: objects.activeObjects) {
            s->draw();
        }

        vtDrawList(drawList);
        drawList.clear();
        vtDisplayFlip();
    }

    // Close signals are ignored during gameplay
    vtWindowLoop();
}

void Game::runMainLoop() {
    while (status != DONE) {
        runOnce();
    }
}
