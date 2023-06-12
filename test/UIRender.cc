#include "TestTools.hh"

#include "engine/virtual/Graphics.hh"
#include "engine/virtual/Window.hh"
#include "ui/loader/UILoader.hh"

int main() {
  vtInitWindow();
  vtInitGraphics();
  auto c = loadUITree("example");
  c->computeLayout();
  DrawList dr;
  dr.clear();
  c->draw(dr);
  vtDrawList(dr);
  vtDisplayFlip();
  vtWindowLoop();
  WANT(vtGetGraphicsError() == 0)
  TEND
}