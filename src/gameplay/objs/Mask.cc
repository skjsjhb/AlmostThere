#include "Mask.hh"

void Mask::draw(DrawContext &ctx)
{
    Polygon pg;
    pg.isOpaque = false;
    vec3 lt = {-1, 1, 0}, rt = {1, 1, 0}, lb = {-1, -1, 0}, rb = {1, -1, 0};
    pg.points.push_back(std::to_array(lt));
    pg.points.push_back(std::to_array(lb));
    pg.points.push_back(std::to_array(rt));
    pg.points.push_back(std::to_array(rb));
    pg.texture = "";
    pg.shader = "mask";
    pg.renderPreset = RECT;
    for (int i = 0; i < 3; ++i)
    {
        pg.args[i] = color[i];
    }
    pg.args[4] = beginRad;

    ctx.polygons.push_back(pg);
}

void Mask::tick(double absTime)
{
}
