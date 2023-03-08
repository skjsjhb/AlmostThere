#include "TestTools.hh"

#include "gameplay/time/Timer.hh"
#include <chrono>
#include <thread>

double implGetTime()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int main()
{
    // Due to hardware limitations the time is not quite accurate
    // So we divided them by 5
    Timer t(implGetTime);
    auto t0 = implGetTime();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    auto t1 = implGetTime();
    WANT((int)(t.getTime() / 5) == (int)((t1 - t0) / 5));

    t.pause();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    t.unpause();

    auto t2 = implGetTime();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    auto t3 = implGetTime();
    WANT((int)(t.getTime() / 5) == (int)((t3 - t2 + t1 - t0) / 5));

    TEND;
}