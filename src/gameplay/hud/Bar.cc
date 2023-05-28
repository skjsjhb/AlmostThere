#include "Bar.hh"

#include "HUDDef.hh"
#include "engine/virtual/Graphics.hh"

#define BAR_HALF_THICK 10

Bar::Bar(BarDirection dirIn, const glm::vec2 &posIn, int maxLengthIn, int widthIn, bool revertDir)
{
    dir = dirIn;
    pos = posIn;
    maxl = maxLengthIn;
    revert = revertDir;
    th = widthIn;
}

void Bar::setLength(double l)
{
    len = l;
}

void Bar::setColor(glm::vec4 c)
{
    color = c;
}

void Bar::flip()
{
    vFlip = !vFlip;
}

void Bar::draw(DrawContext &ctx)
{
    Shape s;
    s.shader = "ui/bar";
    s.texture = "";
    for (int i = 0; i < 4; i++)
    {
        s.args[i] = color[i];
    }
    auto lx = (revert ? -1 : 1) * len * maxl;
    glm::vec3 bl, br, tl, tr;
    switch (dir)
    {
    case VERT:
    {
        bl[0] = tl[0] = pos[0] - th;
        br[0] = tr[0] = pos[0] + th;
        bl[1] = br[1] = pos[1];
        tl[1] = pos[1] + lx + 2 * th * vFlip * sharpEnd;
        tr[1] = pos[1] + lx + 2 * th * (!vFlip) * sharpEnd; // Make a sharp end
    }
    break;
    case HORZ:
    {
        bl[0] = tl[0] = pos[0];
        tr[0] = pos[0] + lx + 2 * th * vFlip * sharpEnd;
        br[0] = pos[0] + lx + 2 * th * (!vFlip) * sharpEnd;
        bl[1] = br[1] = pos[1] - th;
        tl[1] = tr[1] = pos[1] + th;
    }
    break;
    default:
        return;
    }
    tl[2] = bl[2] = tr[2] = br[2] = 0;
    s.points[0] = tl;
    s.points[1] = bl;
    s.points[2] = tr;
    s.points[3] = br;

    ctx.shapes.push_back(s);
}
