#include "TestTools.hh"

#include "lua/LuaSupport.hh"
#include "lua/LuaExt.hh"

static bool isAround(double v1, double v2, double allowDiff) {
    auto diff = std::abs(v2 - v1);
    auto varm = std::max(std::abs(v1), std::abs(v2));
    if (varm <= allowDiff) {
        return diff <= allowDiff;
    } else {
        return diff / varm < allowDiff;
    }
}

int main() {
    luaInit();
    luaSetupExt();
    luaSet("a", 1);
    luaSet("b", 2);
    luaSet("s", "World!");
    auto ind = luaPrecompile("c=__native_qsin(1/b*math.pi)\nes=\"Hello\" .. s\nt={a=\"1\"}");
    luaRunCompiledCode(ind);
    WANT(luaGetInt("a") == 1);
    WANT(isAround(luaGetNumber("c"), 1, 0.01));
    WANT(luaGetStr("es") == "HelloWorld!");
    WANT(luaGetTable("t")["a"] == "1");
    luaClose();
    TEND;
}