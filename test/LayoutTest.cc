#include "TestTools.hh"

#include "ui/layout/Layout.hh"

int main() {
  LayoutUnit t1, t2;

  /*
  ... screen (center)
      40 px
  |-----------|
  |     t1    |
  |----|      |
  | t2 | 50%  |
  |----| ..10 |
  |-----------| ... screen (center)
  */

  t1.baseHorz = t1.baseVert = t1.selfHorz = t1.selfVert = Align::CENTER;
  t1.dx = t2.dx = 0;
  t1.w = t1.h = 40;
  t2.parent = &t1;
  t2.dx = 0;
  t2.dy = 10;
  t2.w = {0.5, LengthMode::PERCENT};
  t2.h = std::string("50%");

  computeLayoutUnit(&t1);
  computeLayoutUnit(&t2);
  auto &r1 = t1.result;
  auto &r2 = t2.result;
  WANT(r2.w == 20)
  WANT(r2.h == 20)
  WANT(r1.x == 780)
  WANT(r1.y == 430)
  WANT(r2.x == 780)
  WANT(r2.y == 440)
  TEND;
}
