#include "TestTools.hh"

#include "gameplay/map/MapLoad.hh"
#include "lua/LuaSupport.hh"
#include "lua/LuaExt.hh"

int main()
{
    luaInit();
    luaSetupExt();
    initMapLoader();
    auto m = loadMap("mapload");
    WANT(m.objects.size() == 1);
    WANT(m.objects[0]->id == "test");
    WANT(m.objects[0]->genTime == 0);
    WANT(m.objects[0]->endTime == 15);
    WANT(m.objects[0]->player == 0);
    WANT(m.objects[0]->type == SLOT);
    TEND;
}