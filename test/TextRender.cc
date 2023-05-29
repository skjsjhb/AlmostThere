#include "TestTools.hh"

#include "engine/virtual/Graphics.hh"
#include "engine/virtual/Window.hh"
#include "gameplay/view/View.hh"
#include "gameplay/base/Game.hh"

int main()
{
    vtInitWindow();
    vtGraphicsInit();
    Typography tp = {
        .text = L"Hello World",
        .pos = {800, 450},
        .color = {1, 1, 1, 1},
        .xAlign = LEFT,
        .yAlign = RIGHT,
    };
    DrawContext dc;
    Game g;
    auto xpt = std::make_shared<Camera>(g);
    dc.cam = xpt;
    dc.typos.push_back(tp);
    vtProcessMeshes(dc);
    vtCompleteDraw(dc);
    vtWindowLoop(); // Run once
    TEND;
};
