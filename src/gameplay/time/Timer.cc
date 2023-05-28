#include "Timer.hh"

Timer::Timer()
{
    _nativeGetTime = nullptr;
    baseTimeOffset = 0;
}

Timer::Timer(NativeTimerFunc f)
{
    _nativeGetTime = f;
    baseTimeOffset = _nativeGetTime();
}

double Timer::getTime()
{
    if (paused)
    {
        return timeBeforePause - baseTimeOffset;
    }
    else
    {
        return innerTime - baseTimeOffset;
    }
}

void Timer::pause()
{
    if (_nativeGetTime != nullptr)
    {
        timeBeforePause = _nativeGetTime();
        paused = true;
    }
}

void Timer::update()
{
    if (_nativeGetTime != nullptr)
    {
        innerTime = _nativeGetTime();
    }
}

void Timer::unpause()
{
    if (_nativeGetTime != nullptr)
    {
        baseTimeOffset += _nativeGetTime() - timeBeforePause;
        paused = false;
    }
}
