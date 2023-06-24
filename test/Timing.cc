#include "TestTools.hh"

#include "gameplay/time/Timer.hh"

static int virtualTime = 0;

double implGetTime() {
  return virtualTime;
}

void timeChange() {
  virtualTime++;
}

int main() {
  Timer t(implGetTime);
  auto t0 = implGetTime();
  timeChange();
  auto t1 = implGetTime();
  t.update();
  WANT(t.getTime() == t1 - t0)

  t.pause();
  timeChange();
  t.update();
  WANT(t.getTime() == t1 - t0) // Should still be the same
  t.unpause();

  auto t2 = implGetTime();
  timeChange();
  auto t3 = implGetTime();
  t.update();
  WANT(t.getTime() == t3 - t2 + t1 - t0)

  t.setSpeed(0.5);
  auto bt = t.getTime();
  auto t4 = implGetTime();
  timeChange();
  t.update();
  auto t5 = implGetTime();
  auto rt = t.getTime();
  WANT(2 * (rt - bt) == t5 - t4)

  t.pause();
  timeChange();
  t.unpause();
  t.update();
  WANT(t.getTime() == rt) // Pause won't change consistency
  TEND
}
