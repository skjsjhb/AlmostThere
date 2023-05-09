#include "TestTools.hh"

#include "lua/LuaExt.hh"
#include "lua/LuaSupport.hh"

#include <chrono>

using namespace std::chrono;

static long long getTime()
{
    return duration_cast<milliseconds>(
               system_clock::now().time_since_epoch())
        .count();
}

int main()
{
    auto t = getTime();
    luaInit();
    luaSetupExt();
    luaSet("a", 0.06368);
    luaRun("for i=1, 1000000 do math.sin(a) end\n");
    auto d1 = getTime() - t;
    t = getTime();
    luaRun("for i=1, 1000000 do __native_qsin(a) end\n");
    auto d2 = getTime() - t;
    WANT(d2 <= d1);
    TEND;
}