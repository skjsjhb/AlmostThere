#ifndef GAMEPLAY_EVENT_SCHEDULE
#define GAMEPLAY_EVENT_SCHEDULE

#include <list>
#include <functional>
#include "gameplay/time/Timer.hh"

typedef std::function<void()> Executor;

class Schedule
{

public:
    void runTaskAt(double time, Executor xt);
    void runTaskLater(double delay, Executor xt);
    void notify(double absTime);
    double getInterval(); // Get MSPT for displaying

protected:
    double _lastNotify = 0, _avgInterval = 0; // Measure tick frequency
    std::list<std::pair<double, Executor>> executors;
};

#endif /* GAMEPLAY_EVENT_SCHEDULE */
