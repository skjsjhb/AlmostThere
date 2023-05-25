#include "Boot.hh"

#include "engine/virtual/Window.hh"
#include "engine/virtual/Graphics.hh"
#include "engine/virtual/Audio.hh"
#include "lua/LuaSupport.hh"
#include "lua/LuaExt.hh"
#include "gameplay/map/MapLoad.hh"
#include "gameplay/control/Controller.hh"
#include "gameplay/base/Game.hh"
#include "user/Account.hh"
#include "gameplay/player/chars/CharList.hh"
#include "spdlog/spdlog.h"

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

void sysInitFull()
{
    spdlog::info("Initializing log module.");
    spdlog::info("Welcome to...");
    spdlog::info(AT_NAME_ARTWORK);
    spdlog::info("System is setting up.");
    // Engine init
    vtInitWindow();
    vtGraphicsInit();
    vtInitAudio();

    // Support env init
    luaInit();
    luaSetupExt();

    // Map Loader
    initMapLoader();

    // Controller
    initControllerLuaExt();
}

void playDemoMap()
{
    auto demoAc = Account::createLocalProfile("Player");
    Game g;
    g.initGame("example");
    g.addPlayer(demoAc, NEKO);
    g.runMainLoop();
}

void sysStop()
{
    spdlog::info("Stopping!");
    luaClose();
    vtCloseAudio();
    vtGraphicsCleanUp();
    vtStopWindow();
    spdlog::info("All systems stopped. 'See you next time'!");
}
