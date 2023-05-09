#include "Schedule.hh"

#include <iostream>

void Schedule::runTaskAt(double time, Executor xt)
{
    auto ia = executors.begin();
    while ((*ia).first < time)
    {
        ia++;
        if (ia == executors.end())
        {
            break;
        }
    }
    executors.insert(ia, {time, xt});
}

void Schedule::runTaskLater(double delay, Executor xt)
{
    Schedule::runTaskAt(delay + _lastNotify, xt);
}

void Schedule::notify(double absTime)
{
    // Let's arrange the interval for better accuracy
    if (_lastNotify == 0)
    {
        _lastNotify = absTime;
        return;
    }
    else
    {
        if (_avgInterval == 0)
        {
            _avgInterval = absTime - _lastNotify;
        }
        else
        {
            // Try to make this more accurate
            _avgInterval = 0.5 * (_avgInterval + absTime - _lastNotify);
        }
        _lastNotify = absTime;
    }
    auto looper = executors.begin();
    while (looper != executors.end())
    {
        auto p = *looper;
        if (p.first - absTime > _avgInterval)
        {
            // All following should be ignored
            return;
        }
        else
        {
            // Run now, even if it might have already been too late
            p.second();
            looper = executors.erase(looper);
        }
    }
}

double Schedule::getInterval()
{
    return _avgInterval;
}