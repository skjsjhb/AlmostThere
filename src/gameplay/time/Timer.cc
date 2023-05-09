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
    if (_nativeGetTime != nullptr)
    {
        if (paused)
        {
            return timeBeforePause - baseTimeOffset;
        }
        else
        {
            return _nativeGetTime() - baseTimeOffset;
        }
    }
    else
    {
        return 0;
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

void Timer::unpause()
{
    if (_nativeGetTime != nullptr)
    {
        baseTimeOffset += _nativeGetTime() - timeBeforePause;
        paused = false;
    }
}