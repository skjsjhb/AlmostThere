#ifndef ALMOSTTHERE_SRC_GAMEPLAY_MAP_LUAMAP_LUAMAPGEN_HH
#define ALMOSTTHERE_SRC_GAMEPLAY_MAP_LUAMAP_LUAMAPGEN_HH

#include "../MapGen.hh"

class Game;

MapData generateLuaMap(const std::string &mapId, Game &g);

void initLuaMapLoader();

#endif // ALMOSTTHERE_SRC_GAMEPLAY_MAP_LUAMAP_LUAMAPGEN_HH
