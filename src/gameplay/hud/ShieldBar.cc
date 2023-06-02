#include "ShieldBar.hh"

#include "gameplay/base/Game.hh"

#define SD_BAR_SIZE 25
#define SD_BAR_XBEGIN 400
#define SD_BAR_MARGIN 5
#define SD_BAR_VALUE 5
#define SD_BAR_YBEGIN 810

void ShieldBar::draw(Game &g)
{
    int numActive = sCurrent / SD_BAR_VALUE;
    int numTotal = sMax / SD_BAR_VALUE;

    for (int i = 0; i < numTotal; i++)
    {
        float xbegin = SD_BAR_XBEGIN + i * (SD_BAR_SIZE + SD_BAR_MARGIN);
        float xend = xbegin + SD_BAR_SIZE;
        float ybegin = SD_BAR_YBEGIN;
        float yend = ybegin + SD_BAR_SIZE;

        if (i < numActive)
        {
            DrawParam p = {
                .shader = "ui/shield-bar",
                .texture = "hud/shield-block-active",
                .ctx = g.ctxUI,
            };
            Rect r(Point({xbegin, yend, 0}, {0, 1}), Point({xbegin, ybegin, 0}, {0, 0}), Point({xend, yend, 0}, {1, 1}), Point({xend, ybegin, 0}, {1, 0}), p);
            g.drawList.add(std::make_unique<Rect>(r));
        }
        else
        {
            DrawParam p = {
                .shader = "ui/shield-bar",
                .texture = "hud/shield-block-empty",
                .ctx = g.ctxUI,
            };
            Rect r(Point({xbegin, yend, 0}, {0, 1}), Point({xbegin, ybegin, 0}, {0, 0}), Point({xend, yend, 0}, {1, 1}), Point({xend, ybegin, 0}, {1, 0}), p);
            g.drawList.add(std::make_unique<Rect>(r));
        }
    }
}
