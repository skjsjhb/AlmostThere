#include "Boot.hh"

#include "engine/virtual/Framework.hh"
#include "engine/virtual/Window.hh"
#include "engine/virtual/Graphics.hh"
#include "engine/virtual/Audio.hh"
#include "engine/virtual/Input.hh"
#include "gameplay/map/MapLoad.hh"
#include "gameplay/player/chars/CharList.hh"
#include "lua/LuaSupport.hh"
#include "lua/LuaExt.hh"
#include "gameplay/base/Game.hh"
#include "spdlog/spdlog.h"
#include "ui/loader/UILoader.hh"

#define AT_NAME_ARTWORK "\n=================================================\n             _                             _\n\
     /\\     | |                           | |\n\
    /  \\    | |  _ __ ___     ___    ___  | |_\n\
   / /\\ \\   | | | '_ ` _ \\   / _ \\  / __| | __|\n\
  / ____ \\  | | | | | | | | | (_) | \\__ \\ | |_ \n\
 /_/    \\_\\ |_| |_| |_| |_|  \\___/  |___/  \\__|\n\
  _______   _\n\
 |__   __| | |\n\
    | |    | |__     ___   _ __    ___\n\
    | |    | '_ \\   / _ \\ | '__|  / _ \\\n\
    | |    | | | | |  __/ | |    |  __/\n\
    |_|    |_| |_|  \\___| |_|     \\___|        \n================================================="

void sysInitFull() {
  spdlog::info("Initializing log module.");
  spdlog::info("Welcome to...");
  spdlog::info(AT_NAME_ARTWORK);
  spdlog::info("Engine is setting up.");

  auto t1 = vtGetTime();

  // Engine init
  vtInitWindow();
  vtInitGraphics();
  vtInitAudio();
  vtInitInput();

  spdlog::info("VMC Engine initialized (" + fmt::format("{:.2f}", vtGetTime() - t1) + "s)!");

  // Support env init
  luaInit();
  luaSetupExt();

  // Map loaders
  initMapLoaders();
}

void playDemoMap() {
  Game g;
  g.setPlayer(DEFAULT);
  g.initGame("example");
  g.runMainLoop();
  if (g.status == FAILED) {
    auto t1 = vtGetTime();
    auto t = loadUITree("failed");
    t->computeLayout();
    DrawList uid;
    t->draw(uid);
    while (vtShouldDraw()) {
      if (vtGetTime() - t1 > 5) {
        break;
      }
      vtDrawList(uid);
      vtDisplayFlip();
    }
  }
}

void sysStop() {
  spdlog::info("Stopping!");
  luaClose();
  vtCloseAudio();
  vtDeInitGraphics();
  vtStopWindow();
  spdlog::info("All systems stopped. 'See you next time'!");
}
