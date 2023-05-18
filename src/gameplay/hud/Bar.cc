#include "Bar.hh"
#include "HUDDef.hh"

#define BAR_HALF_THICK 10

Bar::Bar(BarDirection dirIn, vec2 posIn, int maxLengthIn, int widthIn, bool revertDir)
{
    dir = dirIn;
    glm_vec2_copy(posIn, pos);
    maxl = maxLengthIn;
    revert = revertDir;
    th = widthIn;
}

void Bar::setLength(double l)
{
    len = l;
}

void Bar::setColor(vec4 c)
{
    glm_vec4_copy(c, color);
}

void Bar::flip()
{
    vFlip = !vFlip;
}

void Bar::draw(DrawContext &ctx)
{
    Shape s;
    s.shader = "hud-bar";
    s.texture = "";
    for (int i = 0; i < 4; i++)
    {
        s.args[i] = color[i];
    }
    auto lx = (revert ? -1 : 1) * len * maxl;
    vec3 bl, br, tl, tr;
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
    glm_vec2_copy(tl, s.points[0]);
    glm_vec2_copy(bl, s.points[1]);
    glm_vec2_copy(tr, s.points[2]);
    glm_vec2_copy(br, s.points[3]);

    ctx.shapes.push_back(s);
}