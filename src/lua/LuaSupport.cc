#include "LuaSupport.hh"

extern "C"
{
#include <lauxlib.h>
#include <lualib.h>
}

static lua_State *luaEngine;

void luaInit()
{
    luaEngine = luaL_newstate();
    luaL_openlibs(luaEngine);
}

void luaSet(const std::string &varName, int vInt)
{
    lua_pushinteger(luaEngine, vInt);
    lua_setglobal(luaEngine, varName.c_str());
}

void luaSet(const std::string &varName, double vNumber)
{
    lua_pushnumber(luaEngine, vNumber);
    lua_setglobal(luaEngine, varName.c_str());
}

void luaSet(const std::string &varName, const std::string &vStr)
{
    luaSet(varName, (const char *)vStr.c_str());
}

void luaSet(const std::string &varName, const char *vStr)
{
    lua_pushstring(luaEngine, vStr);
    lua_setglobal(luaEngine, varName.c_str());
}

void luaSet(const std::string &varName, bool vBool)
{
    lua_pushboolean(luaEngine, vBool ? 1 : 0);
    lua_setglobal(luaEngine, varName.c_str());
}

void luaRun(const std::string &code)
{
    luaL_loadstring(luaEngine, code.c_str());
    lua_pcall(luaEngine, 0, 0, 0);
}

int luaGetInt(const std::string &varName)
{
    lua_getglobal(luaEngine, varName.c_str());
    if (lua_isinteger(luaEngine, -1))
    {
        return lua_tointeger(luaEngine, -1);
    }
    else
    {

        return -1;
    }
}

double luaGetNumber(const std::string &varName)
{
    lua_getglobal(luaEngine, varName.c_str());
    if (lua_isnumber(luaEngine, -1))
    {
        return lua_tonumber(luaEngine, -1);
    }
    else
    {
        return -1;
    }
}

std::string luaGetStr(const std::string &varName)
{
    lua_getglobal(luaEngine, varName.c_str());
    if (lua_isstring(luaEngine, -1))
    {
        return std::string(lua_tostring(luaEngine, -1));
    }
    else
    {
        return "";
    }
}

bool luaGetBool(const std::string &varName)
{
    lua_getglobal(luaEngine, varName.c_str());
    if (lua_isboolean(luaEngine, -1))
    {
        return lua_toboolean(luaEngine, -1) == 0 ? false : true;
    }
    else
    {
        return false;
    }
}

void luaBind(const std::string &funName, lua_CFunction func)
{
    lua_pushcfunction(luaEngine, reinterpret_cast<lua_CFunction>(func));
    lua_setglobal(luaEngine, funName.c_str());
}

void luaReset()
{
    lua_settop(luaEngine, 0);
}

void luaClose()
{
    lua_close(luaEngine);
}