#ifndef LUA_LUASUPPORT
#define LUA_LUASUPPORT

#include <string>
#include <functional>

extern "C"
{
#include <lua.h>
}

// Init engine, only need to call once.
void luaInit();

// Assign values
void luaSet(const std::string &varName, int vInt);
void luaSet(const std::string &varName, double vNumber);
void luaSet(const std::string &varName, const std::string &vStr);
void luaSet(const std::string &varName, const char *vStr);
void luaSet(const std::string &varName, bool vBool);

// Bind function
// Make sure you're sending a function pointer!
void luaBind(const std::string &funName, lua_CFunction func);

// Run lua script
void luaRun(const std::string &code);

// Get value

int luaGetInt(const std::string &varName);
double luaGetNumber(const std::string &varName);
std::string luaGetStr(const std::string &varName);
bool luaGetBool(const std::string &varName);

// Reset environment for next run.
void luaReset();

// Stop lua engine
void luaClose();

#endif /* LUA_LUASUPPORT */
