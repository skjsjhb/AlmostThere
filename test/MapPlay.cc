#include "TestTools.hh"
#include "engine/virtual/Graphics.hh"
#include "engine/virtual/Window.hh"
#include "engine/virtual/Framework.hh"
#include "engine/virtual/Input.hh"
#include "gameplay/objs/Note.hh"
#include "gameplay/objs/Mask.hh"
#include "gameplay/base/Game.hh"
#include "lua/LuaSupport.hh"
#include "lua/LuaExt.hh"
#include "gameplay/hud/PlayerBox.hh"
#include "gameplay/hud/Bar.hh"
#include "gameplay/hud/Interact.hh"
#include "user/Account.hh"
#include <cglm/cglm.h>

int main()
{
    vtInitWindow();
    vtGraphicsInit();
    luaInit();
    luaSetupExt();
    initMapLoader();
    initControllerLuaExt();
    Game g;
    auto ac = Account::createLocalProfile("skjsjhb");

    g.initGame("example");
    g.addPlayer(ac, NEKO);
    bool running = true;
    while (running)
    {
        g.runOnce();
    }
    vtGraphicsCleanUp();
    TEND;
}