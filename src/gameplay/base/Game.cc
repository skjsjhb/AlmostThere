#include "Game.hh"

#include "engine/virtual/Input.hh"
#include "gameplay/map/MapLoad.hh"
#include "engine/virtual/Framework.hh"
#include "engine/virtual/Window.hh"
#include "gameplay/player/Player.hh"
#include "user/Account.hh"
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include "lua/LuaSupport.hh"
#include "spdlog/spdlog.h"
#include "support/Resource.hh"

using namespace spdlog;

static View *activeView = nullptr;
static Game *activeGame = nullptr;
static std::string currentCam;

// Active camera interface
static int useCam(lua_State *l) {
  auto cid = lua_tostring(l, -1);
  if (currentCam == cid) {
    return 1; // Not changed
  }
  currentCam = cid;
  if (activeGame != nullptr && activeGame->mapData.content.nameMap.contains(cid)) {
    auto obj = std::dynamic_pointer_cast<Camera>(activeGame->mapData.content.nameMap.at(cid).lock());
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

  mapData = loadMapEx(mapId, *this);

  // Load rules
  rules.enableDetailedScore = true;

  // External resources: background and audio
  if (!mapData.meta.background.empty()) {
    vtSetBackground(getMapResource(mapId, mapData.meta.background));
  } else {
    vtSetBackground("");
  }

  if (!mapData.meta.audio.empty()) {
    audio.bgmBuf = vtLoadAudio(getMapResource(mapId, mapData.meta.audio));
    // Audio playing will be done during each draw
  }

  if (audio.bgmBuf == 0) {
    warn("Missing bgm audio buffer, bgm won't be played.");
  }

  // Init gameplay

  mapObjectPtr = mapData.content.objects.begin();

  // Bind globals for useCam
  activeView = &view;
  activeGame = this;

  int w, h;
  vtGetWindowSize(w, h);
  view.screenSize[0] = float(w);
  view.screenSize[1] = float(h);
  mapTimer = Timer(vtGetTime);
  absTimer = Timer(vtGetTime);

  // Check players
  if (pid > players.size() || pid < 0) {
    error("Invalid player ID specified. This causes undefined behaviour.");
  }
}

void Game::addPlayer(const Account &account, CharID selectedChar) {
  auto epid = players.size() + 1;
  auto xp = Player::createPlayer(selectedChar, account.getUserName(), account.getUID(), epid,
                                 account.getAccountType() == AC_REMOTE);
  if (account.getAccountType() != AC_REMOTE) {
    if (pid != -1) {
      warn("Duplicated local player detected. This causes undefined behaviour.");
    }
    pid = int(epid);
  }
  players.push_back(xp);
}

void Game::runOnce() {
  if (audio.bgmBuf) {
    if (status == RUNNING && !audio.bgmPlaying) {
      vtPlayAudio(audio.bgmBuf);
      audio.bgmPlaying = true;
    }
    if (status != RUNNING && audio.bgmPlaying) {
      vtPauseAudio(audio.bgmBuf);
      audio.bgmPlaying = false;
    }
  }

  mapTimer.update();
  auto mapTimeNow = mapTimer.getTime();

  if (mapTimeNow > mapData.meta.duration) {
    status = DONE;
    return;
  } // TODO: end when squad eliminated


  // Poll inputs
  inputBuf = vtGetInputPoints();

  // Load and unload objects
  while (mapObjectPtr != mapData.content.objects.end()) {
    auto n = *mapObjectPtr;
    if (n->shouldTick()) {
      activeObjects.insert(n);
      mapObjectPtr++;
    } else {
      // If an object is not loaded, the followings won't either.
      break;
    }
  }

  for (auto it = activeObjects.begin(); it != activeObjects.end();) {
    auto n = *it;
    // If an object shouldn't be ticked, it has gone out of scope.
    if (!n->shouldTick()) {
      it = activeObjects.erase(it);
    } else {
      ++it;
    }
  }

  // Tick objects
  for (auto &s : activeObjects) {
    // Judgement of notes have been integrated into draw method, no need to run them again.
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

    for (auto &s : activeObjects) {
      s->draw();
    }

    // Draw HUD
    hudManager.draw();

    // Draw Ambient
    ambient.draw();

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
