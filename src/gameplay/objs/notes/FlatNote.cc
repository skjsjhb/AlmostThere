#include "FlatNote.hh"

#include "NoteTools.hh"
#include "gameplay/base/Game.hh"

void FlatNote::draw()
{
    if (!isActive)
    {
        return;
    }
    auto stat = controller->getState();
    auto &ctx = game.drawContext;

    glm::vec3 pts[4];
    createRect(stat.pos, stat.up, stat.normal, sizew, sizeh, pts);

    Polygon pg = {
        .renderPreset = RECT,
        .isOpaque = true,
        .shader = "3d/rect",
        .texture = name,
        .points = {pts[0], pts[1], pts[2], pts[3]},
    };

    ctx.polygons.push_back(pg);
}
