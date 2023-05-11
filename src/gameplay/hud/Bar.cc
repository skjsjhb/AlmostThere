#include "Bar.hh"
#include "HUDDef.hh"

#define BAR_HALF_THICK 0.015

Bar::Bar(BarDirection dirIn, vec2 posIn, double maxLengthIn, vec4 colorIn, bool revertDir)
{
    dir = dirIn;
    glm_vec2_copy(posIn, pos);
    glm_vec4_copy(colorIn, color);
    maxl = maxLengthIn;
    revert = revertDir;
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
    PolygonShape ps;
    ps.renderPreset = RECT;
    ps.shader = "hud-bar";
    ps.texture = "";
    ps.valuesVec4["color"] = std::to_array(color);
    auto lx = (revert ? -1 : 1) * len * maxl;
    vec3 bl, br, tl, tr;
    auto bh = BAR_HALF_THICK;
    switch (dir)
    {
    case VERT:
    {
        bh = bh / HUD_WH_ASPECT;
        bl[0] = tl[0] = pos[0] - bh;
        br[0] = tr[0] = pos[0] + bh;
        bl[1] = br[1] = pos[1];
        tl[1] = pos[1] + lx + 2 * BAR_HALF_THICK / HUD_WH_ASPECT * vFlip * sharpEnd;
        tr[1] = pos[1] + lx + 2 * BAR_HALF_THICK / HUD_WH_ASPECT * (!vFlip) * sharpEnd; // Make a sharp end
    }
    break;
    case HORZ:
    {
        bl[0] = tl[0] = pos[0];
        tr[0] = pos[0] + lx + 2 * BAR_HALF_THICK * vFlip * sharpEnd;
        br[0] = pos[0] + lx + 2 * BAR_HALF_THICK * (!vFlip) * sharpEnd;
        bl[1] = br[1] = pos[1] - bh;
        tl[1] = tr[1] = pos[1] + bh;
    }
    break;
    default:
        return;
    }
    tl[2] = bl[2] = tr[2] = br[2] = 0;
    ps.points.push_back(std::to_array(tl));
    ps.points.push_back(std::to_array(bl));
    ps.points.push_back(std::to_array(tr));
    ps.points.push_back(std::to_array(br));

    ctx.polygons.push_back(ps);
}