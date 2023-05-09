#ifndef LUA_LUASUPPORT
#define LUA_LUASUPPORT

#include <string>
#include <functional>
#include <map>
#include <vector>

extern "C"
{
#include <lauxlib.h>
#include <lua.h>
}

typedef std::map<std::string, std::string> StringTable;

// Init engine, only need to call once.
void luaInit();

// Assign values
void luaSet(const std::string &varName, int vInt);
void luaSet(const std::string &varName, double vNumber);
void luaSet(const std::string &varName, const std::string &vStr);
void luaSet(const std::string &varName, const char *vStr);
void luaSet(const std::string &varName, bool vBool);

void luaSetExpr(const std::string &expr, int vInt);
void luaSetExpr(const std::string &expr, double vNumber);
void luaSetExpr(const std::string &expr, const std::string &vStr);
void luaSetExpr(const std::string &expr, const char *vStr);
void luaSetExpr(const std::string &expr, bool vBool);

// Bind function
// Make sure you're sending a function pointer!
void luaBind(const std::string &funName, lua_CFunction func);

// Run lua script
void luaRun(const std::string &code);
int luaPrecompile(const std::string &code);
void luaRunCompiledCode(int ref);

// Get value

int luaGetInt(const std::string &varName);
double luaGetNumber(const std::string &varName);
std::string luaGetStr(const std::string &varName);
bool luaGetBool(const std::string &varName);
StringTable luaGetTable(const std::string &varName);

// Get expr value

int luaGetIntExpr(const std::string &expr);
double luaGetNumberExpr(const std::string &expr);
std::string luaGetStrExpr(const std::string &expr);
bool luaGetBoolExpr(const std::string &expr);
StringTable luaGetTableExpr(const std::string &expr);

// Internal method
StringTable luaBuildTable(int index);

// Reset environment for next run.
void luaReset();

// Stop lua engine
void luaClose();

#endif /* LUA_LUASUPPORT */
