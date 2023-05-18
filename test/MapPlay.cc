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

    DrawContext ctx;
    Player *p1 = Player::createPlayer(NEKO, "skjsjhb", 114514, 1, false);
    PlayerBox pb1(p1);
    Interact it1(p1);

    g.initGame("example");
    InputSet isp;
    isp.setupInputListeners();
    bool running = true;
    while (running)
    {
        it1.tick(vtGetTime());
        p1->tick(g);
        pb1.draw(ctx);
        it1.draw(ctx);
        isp.pollInputEvents();
        vtProcessMeshes(ctx);
        vtCompleteDraw(ctx);
        vtWindowLoop();
        ctx.polygons.clear();
        ctx.shapes.clear();
        ctx.typos.clear();
        // g.runOnce();
    }
    delete p1;
    vtGraphicsCleanUp();
    TEND;
}