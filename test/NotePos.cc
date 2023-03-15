#include "TestTools.hh"
#include "gameplay/objs/Note.hh"

int main()
{
    Tapu t;
    Slot s;
    DrawContext ctx;
    t.hitTime = 2;
    t.isFake = false;
    t.isVisible = true;
    t.targetSlot = &s;
    t.autoControl = true;
    s.center[0] = s.center[1] = s.center[2] = 0;
    s.normal[0] = s.normal[1] = 0;
    s.normal[2] = 1;
    s.up[1] = 1;
    s.up[0] = s.up[2] = 0;

    t.tick(4);
    WANT(!t.isVisible);
    t.draw(ctx);
    WANT(ctx.polygons.size() == 0);

    t.isVisible = true; // Manually set
    t.tick(2);
    WANT(t.basePosition[0] == 0 && t.basePosition[1] == 0 && t.basePosition[2] == 0);

    t.tick(1);
    WANT(t.basePosition[0] == 0 && t.basePosition[1] == 2 && t.basePosition[2] == 0);

    t.draw(ctx);
    WANT(ctx.polygons.size() == 1);
    auto pg = ctx.polygons.front();
    WANT(pg.points.size() == 4);

    auto pt0 = pg.points.front();
    WANT(pt0[0] == -0.2f && pt0[1] == 2.2f && pt0[2] == 0.0f);

    TEND;
}