#include <utility>

#include "Timer.hh"

Timer::Timer() {
  _nativeGetTime = nullptr;
  baseTimeOffset = 0;
}

Timer::Timer(NativeTimerFunc f) {
  _nativeGetTime = std::move(f);
  innerTime = innerNativeTime = baseTimeOffset = _nativeGetTime();
}

double Timer::getTime() const {
  if (paused) {
    return timeBeforePause - baseTimeOffset;
  } else {
    return innerTime - baseTimeOffset;
  }
}

void Timer::pause() {
  if (_nativeGetTime != nullptr) {
    timeBeforePause = _nativeGetTime();
    paused = true;
  }
}

void Timer::update() {
  if (paused) {
    return;
  }
  if (_nativeGetTime != nullptr) {
    auto nt = _nativeGetTime();
    auto dt = nt - innerNativeTime; // Native passed time
    innerTime += dt * speed;
    innerNativeTime = nt;
    auto t = getTime();
    for (auto it = timerMap.begin(); it != timerMap.end();) {
      auto &p = *it;
      if (t >= p.first) {
        for (auto &e : p.second) {
          e();
        }
        it = timerMap.erase(it);
      } else {
        break; // Maps are sorted, if the first timer doesn't match, the latters won't either
      }
    }
  }
}

void Timer::unpause() {
  if (_nativeGetTime != nullptr) {
    baseTimeOffset += (_nativeGetTime() - timeBeforePause) * speed;
    paused = false;
  }
}
