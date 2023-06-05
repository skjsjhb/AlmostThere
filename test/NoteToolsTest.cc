#include "TestTools.hh"

#include "gameplay/objs/notes/NoteTools.hh"
#include "gameplay/view/View.hh"
#include "gameplay/base/Game.hh"

using namespace glm;

int main() {
    View v;
    Game g;
    auto cam = std::make_shared<Camera>(g);
    v.camera = cam;
    vec3 pos = {0, 0, 5}, dir = {0, 0, -1}, up = {0, 1, 0};
    v.camera.lock()->setState(pos, dir, up, 90.0, 1);
    vec3 origin[4] = {{-1, -1, 0},
                      {1,  -1, 0},
                      {1,  1,  0},
                      {-1, 1,  0}};
    InputBuffer ip;
    vec2 a = {50, 50};
    ip.touchPoints.push_back(a);
    v.screenSize[0] = 100;
    v.screenSize[1] = 100;
    WANT(isPressed2D(origin, v, ip))

    vec3 origin2[4] = {{0,  1,  0},
                       {-1, 0,  0},
                       {1,  0,  0},
                       {0,  -1, 0}};
    vec2 b = {50, 61};
    InputBuffer ip2;
    ip2.touchPoints.push_back(b);
    WANT(!isPressed2D(origin2, v, ip2))
    TEND
}
