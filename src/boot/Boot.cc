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
#include "support/Locale.hh"
#include "support/Resource.hh"
#include "ui/loader/UILoader.hh"
#include "ui/pages/ResultScreen.hh"

#define AT_NAME_ARTWORK "\n=================================================\n\
             _                             _\n\
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

  // Locale
  setLocale("zh-CN");
}

void playDemoMap() {
  Game g;
  g.setPlayer(DEFAULT);
  g.initGame("example");
  g.runMainLoop();
  if (g.status == FAILED) {
    auto t1 = vtGetTime();
    ResultScreen rs({
                        .variant = ResultScreenVariant::FAILED,
                        .score = g.score.exportScore(),
                        .songTitle = g.mapData.meta.songName,
                        .artist= g.mapData.meta.artist + " // " + g.mapData.meta.mapper,
                        .diffLevel = g.mapData.meta.diffLevel,
                        .playerName = "Player",
                        .banner = getMapResource(g.mapData.meta.id, g.mapData.meta.banner),
                        .diffColor = g.mapData.meta.diffColor,
                        .pf = 0, // TODO apply counts
                        .at = 0,
                        .ac = 0,
                        .md = 0,
                        .tc = 0,
                        .lt = 0,
                    }, vtGetTime());

    while (true) {
      if (vtGetTime() - t1 > 30) {
        break;
      }
      if (vtShouldDraw()) {
        DrawList uid;
        rs.draw(vtGetTime(), uid);
        vtDrawList(uid);
        vtDisplayFlip();
      }
      vtWindowLoop();
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
