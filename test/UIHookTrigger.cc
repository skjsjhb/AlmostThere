#include "TestTools.hh"

#include "engine/virtual/Window.hh"
#include "engine/virtual/Input.hh"
#include "engine/virtual/UIHook.hh"
#include "engine/virtual/Graphics.hh"
#include "gameplay/view/View.hh"

int main()
{
    vtInitWindow();
    vtGraphicsInit();
    InputSet ips;
    bool running = true;
    HookSt s = {
        .xbegin = 800, .xend = 1600, .ybegin = 450, .yend = 900, .xt = [&running]() -> void
        { running = false; }};
    WANT(vtAddUIHook(s) > 0);
    Shape sx = {
        .shader = "hud-bar",
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
    ips.setupInputListeners();
    // Mock a touch point
    std::array ar = {970.0f, 530.0f};
    ips.touchPoints.insert(ar);
    DrawContext ctx;
    auto xpt = std::make_shared<Camera>();
    ctx.cam = xpt;
    while (running)
    {
        ctx.shapes.push_back(sx);
        ctx.typos.push_back(tp);
        // ips.pollInputEvents();
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