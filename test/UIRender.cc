#include "TestTools.hh"

#include "engine/virtual/Framework.hh"
#include "engine/virtual/Graphics.hh"
#include "engine/virtual/Window.hh"
#include "engine/virtual/Input.hh"
#include "ui/comp/impl/Button.hh"
#include "ui/loader/UILoader.hh"

int main() {
  vtInitWindow();
  vtInitGraphics();
  vtInitInput();
  vtSetFPSCap(60);
  auto c = loadUITree("example");
  c->computeLayout();
  DrawList dr;
  if (vtShouldDraw()) {
    dr.clear();
    c->draw(dr);
    vtDrawList(dr);
    vtDisplayFlip();
  }
  vtWindowLoop();
  WANT(vtGetGraphicsError() == 0)
  TEND
}