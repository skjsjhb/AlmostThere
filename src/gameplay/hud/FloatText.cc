#include "FloatText.hh"

#include "gameplay/base/Game.hh"

#define FADE_TIME 0.15
#define FADE_SCALE 2.0f
#define DURATION 2.0
#define FLOAT_TEXT_FONT "mono"

static std::map<FloatTextLevel, glm::vec3> colors = {
    {FloatTextLevel::Normal, {1, 1, 1}},
    {FloatTextLevel::Warning, {1, 0.792, 0.094}},
    {FloatTextLevel::Error, {1, 0.169, 0.251}},
    {FloatTextLevel::Success, {0.690, 1, 0.509}}
};

void FloatText::draw() {
  DrawParam p = {
      .transparent = true,
      .ctx = game.ctxUI,
  };
  float txw = 0;
  for (auto &t : text) {
    float aw, ah;
    vtGetCharSize(FLOAT_TEXT_FONT, t, aw, ah);
    txw += aw;
  }
  txw *= size;
  float pct;
  auto delta = game.mapTimer.getTime() - beginTime;
  if (delta < FADE_TIME) {
    pct = float(delta / FADE_TIME);
  } else if (delta > FADE_TIME + DURATION && delta < 2 * FADE_TIME + DURATION) {
    pct = 1 - float((delta - FADE_TIME - DURATION) / FADE_TIME);
  } else if (delta >= 2 * FADE_TIME + DURATION) {
    pct = 0;
  } else {
    pct = 1;
  }
  auto fds = 1 * pct + FADE_SCALE * (1 - pct);
  glm::vec4 aColor(colors[level], pct);
  DisplayText t(glm::vec2(800 - txw / 2.0 * fds, y), size * fds, text, aColor, FLOAT_TEXT_FONT, p);
  game.drawList.add(t);
}

FloatText::FloatText(Game &g, std::wstring t, float yPos, float sz, FloatTextLevel lv)
    : text(std::move(t)), y(yPos), size(sz), beginTime(g.mapTimer.getTime()), game(g), level(lv) {}
