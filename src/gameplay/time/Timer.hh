#ifndef GAMEPLAY_TIME_TIMER
#define GAMEPLAY_TIME_TIMER

#include "event/Event.hh"
#include <map>
#include <functional>
#include <list>

typedef std::function<double()> NativeTimerFunc;

class Timer {

public:
  Timer();

  explicit Timer(NativeTimerFunc origin);

  [[nodiscard]] double getTime() const;

  void pause();

  void update();

  void unpause();

  /**
   * @brief Change the speed of time.
   *
   * This method sets the time to slow down or speed up during each update or paused. \b AS \b IF the world time
   * was slowed down.
   *
   * @param amp The target speed.
   */
  void setSpeed(double amp) { speed = amp; }

  /**
   * @brief Sets a timeout and execute callback when time meets.
   * @note The timer calculates the time using self timer, which means speed changes will have effect on a set timer.
   * @param t Target time in seconds.
   */
  void setTimeout(double t, const std::function<void()> &cb) { timerMap[getTime() + t].push_back(cb); }

protected:
  double baseTimeOffset, timeBeforePause = 0;
  double innerTime = 0, innerNativeTime = 0;
  double speed = 1.0;
  bool paused = false;
  NativeTimerFunc _nativeGetTime;
  std::map<double, std::list<std::function<void()>>> timerMap;
};

#endif /* GAMEPLAY_TIME_TIMER */
