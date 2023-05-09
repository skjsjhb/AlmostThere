#ifndef GAMEPLAY_MAP_MAPLOAD
#define GAMEPLAY_MAP_MAPLOAD

#include "MapDef.hh"

// Load a map.
GameMap loadMap(const std::string &mapId);

// Free map memory and delete objects.
void freeMap(GameMap &m);

// Init map loader for lua.
void initMapLoader();

#endif /* GAMEPLAY_MAP_MAPLOAD */
