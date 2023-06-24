#include "TestTools.hh"

#include "engine/virtual/Graphics.hh"
#include "engine/virtual/Window.hh"
#include "gameplay/view/View.hh"
#include "gameplay/base/Game.hh"
#include <glm/gtc/matrix_transform.hpp>

int main() {
  vtInitWindow();
  vtInitGraphics();
  DrawParam p = {
      .transparent = true,
      .ctx = {
          .viewMat = glm::mat4(),
          .projMat = glm::ortho(0.0f, 1600.0f, 0.0f, 900.0f),
      },
  };
  DisplayText t1({400, 650}, 0.5, L"hello, world", {1, 1, 1, 1}, "neat", p);
  DisplayText t2({400, 550}, 0.3, L"Credit @ 思源黑体 (Noto Sans)", {1, 1, 1, 1}, "mono", p);
  DisplayText t3({400, 450}, 0.3, L"简体中文 / 繁體中文", {1, 1, 1, 1}, "mono", p);
  DisplayText t4({400, 350}, 0.3, L"SFS A-T Operation Unit : Rhythm Decryption Method", {1, 1, 1, 1}, "styled", p);

  DrawList d;
  Game g;
  auto xpt = std::make_shared<Camera>(g);
  d.add(t1);
  d.add(t2);
  d.add(t3);
  d.add(t4);
  vtDrawList(d);
  vtDisplayFlip();
  vtWindowLoop(); // Run once
  WANT(vtGetGraphicsError() == 0);
  TEND;
};
