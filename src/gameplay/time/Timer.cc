#include "Timer.hh"

Timer::Timer(NativeTimerFunc f)
{
    _nativeGetTime = f;
    baseTimeOffset = _nativeGetTime();
}

double Timer::getTime()
{
    if (paused)
    {
        return timeBeforePause;
    }
    else
    {
        return _nativeGetTime() - baseTimeOffset;
    }
}

void Timer::pause()
{
    timeBeforePause = _nativeGetTime();
    paused = true;
}

void Timer::unpause()
{
    baseTimeOffset += _nativeGetTime() - timeBeforePause;
    paused = false;
}