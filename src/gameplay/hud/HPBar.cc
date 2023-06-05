#include "HPBar.hh"

#include "gameplay/base/Game.hh"

#define HP_BAR_WIDTH 800
#define HP_BAR_XBEGIN 400
#define HP_BAR_YBEGIN 770
#define HP_BAR_HEIGHT 25

#define HP_DRAIN_ADDH 20
#define HP_DRAIN_TIME 0.3
#define HP_HEAL_TIME 0.4

HPDrainSection::HPDrainSection(double valueBegin, double valuePct, double t, HPBarVariant va) {
  beginTime = t;
  xbegin = HP_BAR_XBEGIN + valueBegin * HP_BAR_WIDTH;
  xend = xbegin + int(valuePct * HP_BAR_WIDTH);
  variant = va;
}

void HPDrainSection::draw(Game &g, double absTime) {
  auto l = (absTime - beginTime) / HP_DRAIN_TIME;
  auto alpha = float(1 - l);
  double h = l * HP_DRAIN_ADDH;
  float y1 = HP_BAR_YBEGIN - h;
  float y2 = HP_BAR_YBEGIN + HP_BAR_HEIGHT + h;
  std::vector<float> color;
  if (variant == HP_LOW) {
    color = {1, 0, 0, alpha};
  } else if (variant == HP_NORMAL) {
    color = {1, 1, 1, alpha};
  }
  DrawParam p = {
      .shader = "ui/hp-bar-drain",
      .args = color,
      .ctx = g.ctxUI,
  };

  Rect r(Point({xbegin, y2, 0}), Point({xbegin, y1, 0}), Point({xend, y2, 0}), Point({xend, y1, 0}), p);
  g.drawList.add(std::make_unique<Rect>(r));
}

bool HPDrainSection::shouldUnload(double absTime) const {
  return absTime > (beginTime + HP_DRAIN_TIME);
}

void HPBar::setHP(unsigned int nv, bool animation) {
  auto t = game.mapTimer.getTime();

  if (nv < hpCurrent && animation) {
    auto v1 = nv / double(hpMax);
    auto v2 = (hpCurrent - nv) / double(hpMax);
    if (nv == 0) {
      v2 = 1;
    }
    healEffectBeginTime = -1; // Disable heals
    hpDrainEffects.emplace_back(v1, v2, t, variant);
  } else if (nv > hpCurrent && animation) {
    healEffectBeginTime = t;
  }
  hpCurrent = nv;
}

void HPBar::draw() {
  auto t = game.mapTimer.getTime();
  auto len = (unsigned int) (hpCurrent * HP_BAR_WIDTH / double(hpMax));
  float x1 = HP_BAR_XBEGIN;
  float x2 = HP_BAR_XBEGIN + len;
  float x3 = HP_BAR_XBEGIN + HP_BAR_WIDTH;

  float y1 = HP_BAR_YBEGIN;
  float y2 = HP_BAR_YBEGIN + HP_BAR_HEIGHT;
  /*
  0      2      4
  1      3      5
  */
  auto x2tex = float(len) / float(HP_BAR_HEIGHT);
  Point pts[6] =
      {
          Point({x1, y2, 0}, {0, 1}),
          Point({x1, y1, 0}, {0, 0}),
          Point({x2, y2, 0}, {x2tex, 1}),
          Point({x2, y1, 0}, {x2tex, 0}),
          Point({x3, y2, 0}, {0, 1}),
          Point({x3, y1, 0}, {0, 0}),
      };
  std::string tex;
  if (variant == HP_NORMAL) {
    tex = "hud/hp-bar";
  } else if (variant == HP_LOW) {
    tex = "hud/hp-bar-low";
  }

  float healAmplifier;
  glm::vec4 colorAmplifier = {1, 1, 1, 1};

  // Heal effect
  if ((t - healEffectBeginTime) <= HP_HEAL_TIME) {
    healAmplifier = 1 - float(t - healEffectBeginTime) / float(HP_HEAL_TIME);
    colorAmplifier = glm::vec4(1, 1, 1, 1) * (1 - healAmplifier) + glm::vec4(0.3, 1, 0.3, 1) * healAmplifier;
  }

  // Low HP effect
  if (variant == HP_LOW || hpCurrent / double(hpMax) < 0.5) {
    colorAmplifier = glm::vec4(1, 1, 1, 1) - glm::vec4(0, 1, 1, 0) * float(sin(t * 8) / 2 + 0.5);
  }

  DrawParam parLeft = {
      .shader = "ui/hp-bar",
      .texture = tex + "-fill",
      .args = {colorAmplifier[0], colorAmplifier[1], colorAmplifier[2], colorAmplifier[3]},
      .ctx = game.ctxUI,
  };
  DrawParam parRight = {
      .shader = "ui/hp-bar",
      .texture = tex + "-empty",
      .args = {colorAmplifier[0], colorAmplifier[1], colorAmplifier[2], colorAmplifier[3]},
      .ctx = game.ctxUI,
  };
  Rect hpl(pts[0], pts[1], pts[2], pts[3], parLeft);
  Rect hpr(pts[2], pts[3], pts[4], pts[5], parRight);
  game.drawList.add(std::make_unique<Rect>(hpl));
  game.drawList.add(std::make_unique<Rect>(hpr));

  auto i = hpDrainEffects.begin();
  while (i != hpDrainEffects.end()) {
    if (i->shouldUnload(t)) {
      i = hpDrainEffects.erase(i);
    } else {
      i->draw(game, t);
      ++i;
    }
  }
}
