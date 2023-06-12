#include "TestTools.hh"

#include "engine/virtual/Graphics.hh"
#include "engine/virtual/Window.hh"
#include "gameplay/base/Game.hh"
#include "gameplay/hud/HPBar.hh"
#include "gameplay/hud/ShieldBar.hh"
#include "gameplay/hud/EffectMask.hh"
#include "engine/virtual/Framework.hh"
#include "gameplay/hud/ScoreDisplay.hh"
#include <glm/gtc/matrix_transform.hpp>

int main() {
  vtInitWindow();
  vtInitGraphics();
  vtSetFPSCap(165);
  vtSetTPSCap(1000);
  Game g;
  g.ctxUI = {
      .viewMat = glm::mat4(),
      .projMat = glm::ortho<float>(0, 1600, 0, 900),
  };
  ScoreDisplay sd(g);
  HPBar b(g, HP_NORMAL, 100, 100);

  EffectMask em(g, "hud/effect-mask", "hud/shield-effect");
  EffectMask emb(g, "hud/effect-mask", "hud/blood-effect");

  g.mapTimer = Timer(vtGetTime);
  unsigned int a = 1;
  unsigned hp = 100;
  unsigned score = 50;
  unsigned shield = 50;
  ShieldBar s(g, 50, 25);
  bool revive = false;

  // This task cannot be done automatically. Manually check required.
  while (vtGetTime() < 3) {
    if (vtShouldDraw()) {
      g.mapTimer.update();
      s.draw();
      b.draw();
      sd.draw();
      em.draw();
      emb.draw();
      if (vtGetTime() > a) {
        ++a;
        auto inv = (rand() % 10); // NOLINT Test only

        if (shield > 0) {
          shield = std::max(0, int(shield) - inv);
          em.refresh();
        } else {
          if (!revive) {
            hp = std::max(0, int(hp) - inv);
            emb.refresh();
          }
        }

        b.setHP(hp);
        s.setShield(shield);
        if (hp == 0 && !revive) {
          revive = true;
          b.setVariant(HP_LOW);
          hp = 50;
          b.setHP(50, false);
        }

        score += rand() % 1000; // NOLINT Test only
        sd.setScore(score);
      }
      vtDrawList(g.drawList);
      g.drawList.clear();
      vtDisplayFlip();
      WANT(vtGetGraphicsError() == 0);
    }
    vtWindowLoop();
  }
  TEND
}
