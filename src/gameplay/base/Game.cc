#include "Game.hh"

#include "gameplay/map/MapLoad.hh"
#include "engine/virtual/Framework.hh"
#include "engine/virtual/Window.hh"
#include "gameplay/player/PlayerEvents.hh"
#include "gameplay/player/chars/Default.hh"
#include <glm/gtc/matrix_transform.hpp>
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
    vtPlayAudio(audio.bgmBuf);
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

  // Game process controller

  addEventListener<PlayerDieEvent>([this](PlayerDieEvent &e) -> void {
    if (audio.bgmBuf) {
      vtPauseAudio(audio.bgmBuf);
    }
    mapTimer.setSpeed(0.1); // Make everything seems slower
    mapTimer.setTimeout(0.2, [this]() -> void {
      status = FAILED;
    });
  });

  // Update ortho projection, as they are of constant values
  ctxUI.viewMat = glm::mat4();
  ctxUI.projMat = glm::ortho<float>(0, 1600, 0, 900);
}

void Game::runOnce() {
  mapTimer.update();
  auto mapTimeNow = mapTimer.getTime();
  if (mapTimeNow > mapData.meta.duration) {
    status = DONE;
  }

  if (status == DONE) {
    return;
  }

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
    s->tick();
  }

  if (vtShouldDraw()) {
    if (view.camera.expired()) {
      error("No camera set for game objects drawing. Skipped.");
    }
    // Update draw context
    ctx3D.viewMat = view.camera.lock()->getViewMatrix();
    ctx3D.projMat = view.camera.lock()->getProjectionMatrix();


    // Draw objects
    for (auto &s : activeObjects) {
      s->draw();
    }

    // Draw Ambient
    ambient.draw();

    // Draw HUD
    hudManager.draw();

    vtDrawList(drawList);
    drawList.clear();
    vtDisplayFlip();
  }

  // Close signals are ignored during gameplay
  vtWindowLoop();
}

void Game::runMainLoop() {
  while (status == RUNNING) {
    runOnce();
  }
}
void Game::setPlayer(CharID ch) {
  switch (ch) {
  case DEFAULT:player = std::make_shared<DefaultChar>(*this);
    break;
  default:break;
  }
  player->addEventListeners();
}

Game::~Game() {
  if (player) {
    player->removeEventListeners();
  }
}