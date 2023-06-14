#include "EffectMask.hh"

#include "gameplay/base/Game.hh"

#define MASK_PERSIST_TIME 0.2
#define MASK_FADE_TIME 0.4

void EffectMask::refresh() {
  beginTime = game.mapTimer.getTime();
}

void EffectMask::draw() const {
  auto t = game.mapTimer.getTime();
  if (t > beginTime + MASK_PERSIST_TIME + MASK_FADE_TIME) {
    return;
  }
  float alpha;
  if (t < beginTime + MASK_PERSIST_TIME) {
    alpha = 1;
  } else {
    alpha = float(std::max(0.0, 1 - (t - beginTime - MASK_PERSIST_TIME) / MASK_FADE_TIME));
  }
  DrawParam p = {
      .shader = shader,
      .texture = {texture},
      .transparent = true,
      .args = {alpha},
      .ctx = game.ctxUI,
  };
  Rect r(Point({0, 900, 0}, {0, 1}), Point({0, 0, 0}, {0, 0}), Point({1600, 900, 0}, {1, 1}),
         Point({1600, 0, 0}, {1, 0}), p);
  game.drawList.add(r);
}
