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
    Player p = {
        .playerName = "skjsjhb",
        .charName = "NEKO",
        .shield = {
            .maxHP = 100,
            .curHP = 50,

        },
        .health = 100,
        .maxHealth = 100,
    };
    PlayerBox pb(&p);
    g.initGame("example");
    bool running = true;

    while (running)
    {
        pb.draw(ctx);
        vtProcessMeshes(ctx);
        vtCompleteDraw(ctx);
        vtWindowLoop();
        ctx.polygons.clear();
        ctx.typos.clear();
        // g.runOnce();
    }
    vtGraphicsCleanUp();
    TEND;
}