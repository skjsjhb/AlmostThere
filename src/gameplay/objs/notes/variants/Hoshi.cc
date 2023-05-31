#include "Hoshi.hh"

#include "gameplay/base/Game.hh"
#include "util/Util.hh"
#include <glm/glm.hpp>

void Hoshi::draw()
{
    if (!isActive)
    {
        return;
    }

    /*
               N[0](0,1)
    L[2](1,0) F[4](0,0) B[5](0,0) R[3](1,0)
               S[1](0,1)
    */

    Point points[6]; // NS, LR, FB

    auto stat = controller->getState();
    auto rightVec = glm::normalize(glm::cross(stat.up, stat.normal)) * float(sizew);

    auto upVec = stat.up * float(sizew);
    auto normVec = stat.normal * float(sizew);

    points[0] = {stat.pos + normVec, {0, 1}};
    points[1] = {stat.pos - normVec, {0, 1}};

    points[2] = {stat.pos - rightVec, {1, 0}};
    points[3] = {stat.pos + rightVec, {1, 0}};
    points[4] = {stat.pos - upVec, {0, 0}};
    points[5] = {stat.pos + upVec, {0, 0}};

    DrawParam p = {
        .shader = "3d/mesh",
        .texture = "hoshi",
        .ctx = game.ctx3D,
    };

    TriangleStrip fts(strip(points, {4, 3, 0, 4, 0, 2, 4, 2, 1, 4, 1, 3}), p);
    TriangleStrip bts(strip(points, {5, 2, 0, 5, 0, 3, 5, 3, 1, 5, 1, 2}), p);

    game.drawList.add(std::make_unique<TriangleStrip>(fts));
    game.drawList.add(std::make_unique<TriangleStrip>(bts));
}
