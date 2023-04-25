#include "LuaExt.hh"

#define _USE_MATH_DEFINES
#include <math.h>
#include <map>
#include "LuaSupport.hh"

static double sinValue[360], cosValue[360];

static int stdDeg(double rad)
{
    int refDeg = static_cast<int>(rad / (2 * M_PI) * 360) % 360;
    if (refDeg < 0)
    {
        refDeg = 360 + refDeg;
    }
    return refDeg;
}

static int quickSin(lua_State *l)
{
    double rad = lua_tonumber(l, -1);
    lua_pushnumber(l, sinValue[stdDeg(rad)]);
    return 1;
}

static int quickCos(lua_State *l)
{
    double rad = lua_tonumber(l, -1);
    lua_pushnumber(l, cosValue[stdDeg(rad)]);
    return 1;
}

static int quickTan(lua_State *l)
{
    double rad = lua_tonumber(l, -1);
    auto i = stdDeg(rad);
    lua_pushnumber(l, sinValue[i] / cosValue[i]);
    return 1;
}

void luaSetupExt()
{
    for (int i = 0; i < 360; i++)
    {
        sinValue[i] = std::sin(i / 180.0 * M_PI);
        cosValue[i] = std::cos(i / 180.0 * M_PI);
    }
    luaBind("qsin", quickSin);
    luaBind("qcos", quickCos);
    luaBind("qtan", quickTan);
}