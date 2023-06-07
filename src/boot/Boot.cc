#include "Boot.hh"

#include "engine/virtual/Window.hh"
#include "engine/virtual/Graphics.hh"
#include "engine/virtual/Audio.hh"
#include "lua/LuaSupport.hh"
#include "lua/LuaExt.hh"
#include "gameplay/base/Game.hh"
#include "user/Account.hh"
#include "spdlog/spdlog.h"
#include "gameplay/map/luamap/LuaMapGen.hh"

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
  spdlog::info("System is setting up.");
  // Engine init
  vtInitWindow();
  vtInitGraphics();
  vtInitAudio();

  // Support env init
  luaInit();
  luaSetupExt();

  // Lua Map Loader
  initLuaMapLoader();

  // Controller
  // initControllerLuaExt();
}

void playDemoMap() {
  auto demoAc = Account::createLocalProfile("Player");
  Game g;
  g.addPlayer(demoAc, NEKO);
  g.initGame("example");
  g.runMainLoop();
}

void sysStop() {
  spdlog::info("Stopping!");
  luaClose();
  vtCloseAudio();
  vtDeInitGraphics();
  vtStopWindow();
  spdlog::info("All systems stopped. 'See you next time'!");
}
