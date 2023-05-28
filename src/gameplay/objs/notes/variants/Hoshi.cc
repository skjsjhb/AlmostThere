#include "Hoshi.hh"

#include "gameplay/base/Game.hh"
#include <glm/glm.hpp>

void Hoshi::draw()
{
    if (!isActive)
    {
        return;
    }

    glm::vec3 points[6]; // NS, LR, FB
    auto stat = controller->getState();
    auto rightVec = glm::normalize(glm::cross(stat.up, stat.normal)) * float(sizew);

    auto upVec = stat.up * float(sizew);
    auto normVec = stat.normal * float(sizew);

    points[3] = stat.pos + rightVec;
    points[5] = stat.pos + upVec;
    points[4] = stat.pos - upVec;
    points[2] = stat.pos - rightVec;
    points[0] = stat.pos + normVec;
    points[1] = stat.pos - normVec;

    Polygon ps;
    ps.renderPreset = OCT;
    ps.shader = "3d/hoshi";
    ps.texture = "hoshi";
    for (int i = 0; i < 6; i++)
    {
        ps.points.push_back(points[i]);
    }
    game.drawContext.polygons.push_back(ps);
}
