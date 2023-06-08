#include "ShieldBar.hh"

#include "gameplay/base/Game.hh"

#define SD_BAR_TW 800
#define SD_BAR_H 12
#define SD_BAR_XBEGIN 400
#define SD_BAR_MARGIN 5
#define SD_BAR_VALUE 5
#define SD_BAR_YBEGIN 820
#define SD_BAR_SHIFT 4

void ShieldBar::draw() const {
  unsigned int numActive = sCurrent / SD_BAR_VALUE;
  unsigned int numTotal = sMax / SD_BAR_VALUE;

  float w = SD_BAR_TW / double(numTotal) - SD_BAR_MARGIN;

  for (int i = 0; i < numTotal; i++) {
    float xbegin = SD_BAR_XBEGIN + float(i) * (w + SD_BAR_MARGIN);
    float xend = xbegin + w;
    float ybegin = SD_BAR_YBEGIN;
    float yend = ybegin + SD_BAR_H;

    if (i < numActive) {
      DrawParam p = {
          .shader = "ui/shield-bar",
          .texture = "hud/shield-block-active",
          .ctx = game.ctxUI,
      };
      Rect r(Point({xbegin + float(i == 0 ? 0 : SD_BAR_SHIFT), yend, 0}, {0, 1}),
             Point({xbegin, ybegin, 0}, {0, 0}),
             Point({xend + SD_BAR_SHIFT, yend, 0}, {1, 1}),
             Point({xend, ybegin, 0}, {1, 0}),
             p);
      game.drawList.add(r);
    } else {
      DrawParam p = {
          .shader = "ui/shield-bar",
          .texture = "hud/shield-block-empty",
          .ctx = game.ctxUI,
      };
      Rect r(Point({xbegin + float(i == 0 ? 0 : SD_BAR_SHIFT), yend, 0}, {0, 1}),
             Point({xbegin, ybegin, 0}, {0, 0}),
             Point({xend + SD_BAR_SHIFT, yend, 0}, {1, 1}),
             Point({xend, ybegin, 0}, {1, 0}),
             p);
      game.drawList.add(r);
    }
  }
}
