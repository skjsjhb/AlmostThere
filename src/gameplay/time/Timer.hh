#ifndef GAMEPLAY_TIME_TIMER
#define GAMEPLAY_TIME_TIMER

#include <functional>

typedef std::function<double()> NativeTimerFunc;

class Timer
{

public:
    Timer();
    Timer(NativeTimerFunc origin);
    double getTime();
    void pause();
    void update();
    void unpause();

protected:
    double baseTimeOffset, timeBeforePause = 0;
    double innerTime = 0;
    bool paused = false;
    NativeTimerFunc _nativeGetTime;
};

#endif /* GAMEPLAY_TIME_TIMER */
