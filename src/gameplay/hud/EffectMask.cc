#include "EffectMask.hh"

#include "engine/virtual/Graphics.hh"
#include "gameplay/base/Game.hh"
#include "ui/comp/Component.hh"

#define MASK_PULSE_PERSISIT_TIME 0.2
#define MASK_PULSE_FADE_TIME 0.4
#define MASK_GROW_IN_TIME 0.01
#define MASK_GROW_PERSIST_TIME 0.24

#define MASK_FADE_TIME 1.5
#define MASK_FADE_PERSIST_TIME 1.0

void EffectMask::refresh(double absTime) {
  beginTime = absTime;
}

void EffectMask::draw(double t, DrawList &d) const {
  float alpha;
  switch (variant) {
  case EffectMaskVariant::PULSE:
    if (t > beginTime + MASK_PULSE_PERSISIT_TIME + MASK_PULSE_FADE_TIME) {
      return;
    }
    if (t < beginTime + MASK_PULSE_PERSISIT_TIME) {
      alpha = 1;
    } else {
      alpha = float(std::max(0.0, 1 - (t - beginTime - MASK_PULSE_PERSISIT_TIME) / MASK_PULSE_FADE_TIME));
    }
    break;
  case EffectMaskVariant::GROW:

    if (t > beginTime + MASK_GROW_IN_TIME + MASK_GROW_PERSIST_TIME) {
      return;
    }
    if (t > beginTime + MASK_GROW_IN_TIME) {
      alpha = 1;
    } else {
      alpha = float(std::min(1.0, (t - beginTime) / MASK_GROW_IN_TIME));
    }
    break;
  case EffectMaskVariant::FADE:
    if (t > beginTime + MASK_FADE_PERSIST_TIME + MASK_FADE_TIME) {
      return;
    }
    if (t < beginTime + MASK_FADE_PERSIST_TIME) {
      alpha = 1;
    } else {
      alpha = 1 - float(std::min(1.0, (t - beginTime - MASK_FADE_PERSIST_TIME) / MASK_FADE_TIME));
    }
    break;
  case EffectMaskVariant::STATIC:alpha = 1;
    break;
  }

  DrawParam p = {
      .shader = shader,
      .texture = {texture},
      .transparent = true,
      .args = {alpha},
      .ctx = getDefaultDrawContext(),
  };
  Rect r(Point({0, 900, 0}, {0, 1}), Point({0, 0, 0}, {0, 0}), Point({1600, 900, 0}, {1, 1}),
         Point({1600, 0, 0}, {1, 0}), p);
  d.add(r);
}
