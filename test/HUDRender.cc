#include "TestTools.hh"

#include "engine/virtual/Graphics.hh"
#include "engine/virtual/Window.hh"
#include "gameplay/base/Game.hh"
#include "engine/virtual/Framework.hh"
#include "gameplay/hud/EffectMask.hh"
#include "gameplay/hud/TextBox.hh"
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

  TextBox tb(g, glm::vec4(0.83, 0, 0.16, 0.6), 100);
  tb.setText(L"UNSTABLE CORE");
  tb.setIndex(0);

  TextBox tb2(g, glm::vec4(0.549, 1, 0.984, 0.6), 100);
  tb2.setText(L"NEKO SENSE: 07.22");
  tb2.setIndex(1);

  FloatText ft(g, L"SHIELD CHARGED", 700, 0.4);

  FloatText ft2(g, L"Recharge Complete.", 650, 0.2);
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
      tb.draw();
      tb2.draw();
      ft.draw();
      ft2.draw();
      if (vtGetTime() > a) {
        ++a;
        auto inv = (rand() % 10); // NOLINT Test only

        if (shield > 0) {
          shield = std::max(0, int(shield) - inv);
        } else {
          if (!revive) {
            hp = std::max(0, int(hp) - inv);
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
