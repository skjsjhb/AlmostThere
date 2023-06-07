#include "TestTools.hh"

#include "gameplay/map/MapLoad.hh"
#include "gameplay/base/Game.hh"
#include "lua/LuaSupport.hh"
#include "lua/LuaExt.hh"

int main() {
  luaInit();
  luaSetupExt();
  initMapLoaders();
  Game g;
  auto m = loadMapEx("mapload", g);
  WANT(m.content.objects.size() == 1)
  WANT(m.meta.id == "mapload");
  TEND
}