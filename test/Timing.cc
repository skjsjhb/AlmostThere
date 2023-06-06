#include "TestTools.hh"

#include "gameplay/time/Timer.hh"
#include <chrono>
#include <thread>

static int virtualTime = 0;

double implGetTime() {
  return virtualTime;
}

void timeChange() {
  virtualTime += rand() % 0xfffff; // NOLINT Test only
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

  TEND
}
