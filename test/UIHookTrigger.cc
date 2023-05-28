#include "TestTools.hh"

#include "engine/virtual/Window.hh"
#include "engine/virtual/Input.hh"
#include "engine/virtual/UIHook.hh"
#include "engine/virtual/Graphics.hh"
#include "gameplay/view/View.hh"
#include "gameplay/base/Game.hh"

int main()
{
    Game g;
    vtInitWindow();
    vtGraphicsInit();
    InputBuffer ips;
    bool running = true;
    HookSt s = {
        .xbegin = 800, .xend = 1600, .ybegin = 450, .yend = 900, .xt = [&running]() -> void
        { running = false; }};
    WANT(vtAddUIHook(s) > 0);
    Shape sx = {
        .shader = "ui/bar",
        .texture = "",
        .points = {
            {800, 900}, {800, 450}, {1600, 900}, {1600, 450}},
        .args = {1, 1, 1, 1},
    };
    Typography tp = {
        .text = L"Click To Close",
        .pos = {1200, 675},
        .color = {0, 0, 0, 1},
        .size = 0.5,
        .xAlign = CENTER,
        .yAlign = CENTER,
    };
    vtSetupListeners();
    // Mock a touch point
    ips.touchPoints.push_back({970.0f, 530.0f});
    DrawContext ctx;
    auto xpt = std::make_shared<Camera>(g);
    ctx.cam = xpt;
    while (running)
    {
        ctx.shapes.push_back(sx);
        ctx.typos.push_back(tp);
        vtNotifyUIHooks(ips); // Should not be called. Only for testing!
        WANT(running == false);
        vtProcessMeshes(ctx);
        vtCompleteDraw(ctx);
        vtWindowLoop();
        ctx.shapes.clear();
        ctx.typos.clear();
    }
    TEND;
}
