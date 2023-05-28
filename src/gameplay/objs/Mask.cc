#include "Mask.hh"

#include "gameplay/base/Game.hh"

void Mask::draw()
{
    Polygon pg;
    pg.isOpaque = false;
    glm::vec3 lt = {-1, 1, 0}, rt = {1, 1, 0}, lb = {-1, -1, 0}, rb = {1, -1, 0};
    pg.points = {lt, rt, lb, rb};
    pg.texture = "";
    pg.shader = "ui/mask";
    pg.renderPreset = RECT;
    for (int i = 0; i < 3; ++i)
    {
        pg.args[i] = color[i];
    }
    pg.args[4] = beginRad;
    game.drawContext.polygons.push_back(pg);
}

void Mask::tick()
{
}
