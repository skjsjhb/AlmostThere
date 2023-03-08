#ifndef GAMEPLAY_TIME_TIMER
#define GAMEPLAY_TIME_TIMER

#include <functional>

typedef std::function<double()> NativeTimerFunc;

class Timer
{
protected:
    double baseTimeOffset, timeBeforePause = 0;
    bool paused = false;
    NativeTimerFunc _nativeGetTime;

public:
    Timer(NativeTimerFunc origin);
    double getTime();
    void pause();
    void unpause();
};

#endif /* GAMEPLAY_TIME_TIMER */
