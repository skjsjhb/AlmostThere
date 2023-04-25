#include "TestTools.hh"

#include "lua/LuaSupport.hh"
#include "lua/LuaExt.hh"

int main()
{
    luaInit();
    luaSetupExt();
    luaSet("a", 1);
    luaSet("b", 2);
    luaSet("s", "World!");
    luaRun("c=qsin(1/b*math.pi)\nes=\"Hello\" .. s");
    WANT(luaGetInt("a") == 1);
    WANT(luaGetNumber("c") == 1);
    WANT(luaGetStr("es") == std::string("HelloWorld!"));
    luaClose();
    TEND;
}