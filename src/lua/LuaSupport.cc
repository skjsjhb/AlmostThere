#include "LuaSupport.hh"

extern "C"
{
#include <lauxlib.h>
#include <lualib.h>
}

#include "support/Resource.hh"
#include "spdlog/spdlog.h"

using namespace spdlog;

static lua_State *luaEngine;

void luaInit() {
    info("Setting up lua engine.");
    // Init engine
    luaEngine = luaL_newstate();
    luaL_openlibs(luaEngine);
    // Set libs search path
    auto libPt = getAppResource("lua/?.lua");
    lua_pushstring(luaEngine, libPt.c_str());
    lua_setglobal(luaEngine, "__native_lualibs_path");
    luaL_dostring(luaEngine, "package.path = __native_lualibs_path\n__native_lualibs_path=nil");
    auto libPf = getAppResource("lua/preload.lua");
    luaL_dofile(luaEngine, libPf.c_str());
}

void luaSet(const std::string &varName, int vInt) {
    lua_pushinteger(luaEngine, vInt);
    lua_setglobal(luaEngine, varName.c_str());
}

void luaSet(const std::string &varName, double vNumber) {
    lua_pushnumber(luaEngine, vNumber);
    lua_setglobal(luaEngine, varName.c_str());
}

void luaSet(const std::string &varName, const std::string &vStr) {
    luaSet(varName, (const char *) vStr.c_str());
}

void luaSet(const std::string &varName, const char *vStr) {
    lua_pushstring(luaEngine, vStr);
    lua_setglobal(luaEngine, varName.c_str());
}

void luaSet(const std::string &varName, bool vBool) {
    lua_pushboolean(luaEngine, vBool ? 1 : 0);
    lua_setglobal(luaEngine, varName.c_str());
}

#define DUMMY_VAR_NAME "__native_dummy"

void luaSetExpr(const std::string &expr, int vInt) {
    luaSet(DUMMY_VAR_NAME, vInt);
    luaRun(expr + "=" + DUMMY_VAR_NAME);
}

void luaSetExpr(const std::string &expr, double vNumber) {
    luaSet(DUMMY_VAR_NAME, vNumber);
    luaRun(expr + "=" + DUMMY_VAR_NAME);
}

void luaSetExpr(const std::string &expr, const std::string &vStr) {
    luaSet(DUMMY_VAR_NAME, vStr);
    luaRun(expr + "=" + DUMMY_VAR_NAME);
}

void luaSetExpr(const std::string &expr, const char *vStr) {
    luaSet(DUMMY_VAR_NAME, vStr);
    luaRun(expr + "=" + DUMMY_VAR_NAME);
}

void luaSetExpr(const std::string &expr, bool vBool) {
    luaSet(DUMMY_VAR_NAME, vBool);
    luaRun(expr + "=" + DUMMY_VAR_NAME);
}

static void handleErr(int err) {
    if (err != 0) {
        switch (err) {
            case LUA_ERRMEM:
                error("Lua interpreter reported insufficient memory. Are there memory leaks?");
                break;
            case LUA_ERRSYNTAX:
                error("Lua syntax error detected. Check your code.");
                break;
            case LUA_ERRRUN:
                error("Lua runtime error detected. See message below.");
                break;
            default:
                break;
        }
        if (lua_isstring(luaEngine, -1)) {
            error("Lua interpreter reported error message:");
            error(lua_tostring(luaEngine, -1));
        } else {
            error("No error message is available. Is stack corrupted?");
        }
        lua_pop(luaEngine, 1); // Remove the err object
    }
}

void luaRun(const std::string &code) {
    luaL_loadstring(luaEngine, code.c_str());
    auto err = lua_pcall(luaEngine, 0, 0, 0); // No ret value
    handleErr(err);
}

#define LUA_PRECOMPILE_FUN_TBL_NAME "__native_precompile"

int luaPrecompile(const std::string &code) {
    lua_getglobal(luaEngine, LUA_PRECOMPILE_FUN_TBL_NAME);
    if (!lua_istable(luaEngine, -1)) {
        lua_pop(luaEngine, 1);
        lua_newtable(luaEngine);
        lua_setglobal(luaEngine, LUA_PRECOMPILE_FUN_TBL_NAME);
        lua_getglobal(luaEngine, LUA_PRECOMPILE_FUN_TBL_NAME);
    }
    int id, err;
    if ((err = luaL_loadstring(luaEngine, code.c_str())) != LUA_OK) {
        warn("Compilation error detected. See message below.");
        switch (err) {
            case LUA_ERRSYNTAX:
                warn("Syntax error detected. Check the script or its loader.");
                break;
            case LUA_ERRMEM:
                warn("Insufficient memory detected. Is there any memory leak, or high-consuming functions called?");
                break;
            default:
                warn("Unknown error occurred. Check if Lua library is corrupted.");
                break;
        }
        id = LUA_NOREF;
    } else {
        id = luaL_ref(luaEngine, -2);
    }
    lua_pop(luaEngine, 1);
    return id;
}

void luaRunCompiledCode(int ref) {
    if (ref == LUA_NOREF) {
        warn("Attempting to run compiled code with invalid ref. Skipped.");
        return;
    }
    lua_getglobal(luaEngine, LUA_PRECOMPILE_FUN_TBL_NAME);
    if (!lua_istable(luaEngine, -1)) {
        warn("Compiled code buffer not found.");
        lua_pop(luaEngine, 1);
        return;
    }
    if (lua_rawgeti(luaEngine, -1, ref) != LUA_TFUNCTION) {
        warn("Invalid ref to a non-existing function.");
        lua_pop(luaEngine, 2);
        return;
    }
    auto err = lua_pcall(luaEngine, 0, 0, 0); // No ret value
    handleErr(err);
    lua_pop(luaEngine, 1); // Remove the table
}

int luaGetInt(const std::string &varName) {
    lua_getglobal(luaEngine, varName.c_str());
    int r = 0;
    if (lua_isinteger(luaEngine, -1)) {
        r = lua_tointeger(luaEngine, -1);
    } else {
        warn("Incompatible Lua types detected. Reading '" + varName + "' as INT. It is " +
             lua_typename(luaEngine, lua_type(luaEngine, -1)));
    }
    lua_remove(luaEngine, -1);
    return r;
}

double luaGetNumber(const std::string &varName) {

    lua_getglobal(luaEngine, varName.c_str());
    double r = 0;
    if (lua_isnumber(luaEngine, -1)) {
        r = lua_tonumber(luaEngine, -1);
    } else {
        warn("Incompatible Lua types detected. Reading '" + varName + "' as NUMBER. It is " +
             lua_typename(luaEngine, lua_type(luaEngine, -1)));
    }
    lua_remove(luaEngine, -1);
    return r;
}

std::string luaGetStr(const std::string &varName) {
    lua_getglobal(luaEngine, varName.c_str());
    std::string r;
    if (lua_isstring(luaEngine, -1)) {
        r = lua_tostring(luaEngine, -1);
    } else {
        warn("Incompatible Lua types detected. Reading '" + varName + "' as STR. It is " +
             lua_typename(luaEngine, lua_type(luaEngine, -1)));
    }
    lua_remove(luaEngine, -1);
    return r;
}

bool luaGetBool(const std::string &varName) {
    lua_getglobal(luaEngine, varName.c_str());
    bool r = false;
    if (lua_isboolean(luaEngine, -1)) {
        r = lua_toboolean(luaEngine, -1) != 0;
    } else {
        warn("Incompatible Lua types detected. Reading '" + varName + "' as BOOL. It is " +
             lua_typename(luaEngine, lua_type(luaEngine, -1)));
    }
    lua_remove(luaEngine, -1);
    return r;
}

StringTable luaBuildTable(int index) {
    StringTable tbl;
    if (lua_istable(luaEngine, index)) {
        lua_pushnil(luaEngine);
        while (lua_next(luaEngine, index - 1)) {
            lua_pushvalue(luaEngine, index - 1);
            auto key = lua_tostring(luaEngine, index);
            auto value = lua_tostring(luaEngine, index - 1);
            tbl[key] = value;
            lua_pop(luaEngine, 2);
        }
    }
    return tbl;
}

StringTable luaGetTable(const std::string &varName) {
    lua_getglobal(luaEngine, varName.c_str());
    if (lua_istable(luaEngine, -1)) {
        return luaBuildTable(-1);
    } else {
        warn("Incompatible Lua types detected. Reading '" + varName + "' as TABLE. It is " +
             lua_typename(luaEngine, lua_type(luaEngine, -1)));
    }
    lua_remove(luaEngine, -1);
    return {};
}

#define LUA_RUN_EXPR (luaRun(std::string(DUMMY_VAR_NAME) + "=(" + expr + ")"))

int luaGetIntExpr(const std::string &expr) {
    LUA_RUN_EXPR;
    return luaGetInt(DUMMY_VAR_NAME);
}

double luaGetNumberExpr(const std::string &expr) {
    LUA_RUN_EXPR;
    return luaGetNumber(DUMMY_VAR_NAME);
}

std::string luaGetStrExpr(const std::string &expr) {
    LUA_RUN_EXPR;
    return luaGetStr(DUMMY_VAR_NAME);
}

bool luaGetBoolExpr(const std::string &expr) {
    LUA_RUN_EXPR;
    return luaGetBool(DUMMY_VAR_NAME);
}

StringTable luaGetTableExpr(const std::string &expr) {
    LUA_RUN_EXPR;
    return luaGetTable(DUMMY_VAR_NAME);
}

void luaBind(const std::string &funName, lua_CFunction func) {
    lua_pushcfunction(luaEngine, reinterpret_cast<lua_CFunction>(func));
    lua_setglobal(luaEngine, funName.c_str());
}

void luaReset() {

    lua_settop(luaEngine, 0);
}

void luaClose() {
    lua_close(luaEngine);
}
