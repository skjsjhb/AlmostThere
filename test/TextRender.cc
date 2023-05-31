#include "TestTools.hh"

#include "engine/virtual/Graphics.hh"
#include "engine/virtual/Window.hh"
#include "gameplay/view/View.hh"
#include "gameplay/base/Game.hh"
#include <glm/gtc/matrix_transform.hpp>

int main()
{
    vtInitWindow();
    vtInitGraphics();
    DrawParam p = {
        .ctx = {
            .viewMat = glm::mat4(),
            .projMat = glm::ortho(0.0f, 1600.0f, 0.0f, 900.0f),
        },
    };
    DisplayText t({800, 450}, 1, L"hello, world", {1, 1, 1, 1}, p);
    DrawList d;
    Game g;
    auto xpt = std::make_shared<Camera>(g);
    d.add(std::make_unique<DisplayText>(t));
    while (true)
    {
        vtDrawList(d);
        vtWindowLoop(); // Run once
    }
    TEND;
};
