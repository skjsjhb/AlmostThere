#include "gameplay/event/Schedule.hh"

#include "TestTools.hh"

int main()
{
    Schedule s, a;
    a.notify(1);
    a.notify(2);
    WANT(a.getInterval() == 1);
    int cl = 0;
    auto addCL = [&cl]() -> void
    { cl++; };
    s.runTaskLater(5, addCL);
    s.runTaskLater(5, addCL);
    s.runTaskLater(10, addCL);
    s.runTaskLater(15, addCL);
    for (int i = 1; i <= 10; i++)
    {
        s.notify(i);
    }
    WANT(cl == 3);
    WANT(s.getInterval() == 1);
    TEND;
}