#ifndef ALMOSTTHERE_SRC_GAMEPLAY_MAP_MAPLOADGENERIC_HH
#define ALMOSTTHERE_SRC_GAMEPLAY_MAP_MAPLOADGENERIC_HH

#include "MapGen.hh"

#include <string>

class Game;

MapData loadMapEx(const std::string &mapId, Game &g);

/**
 * @brief Initialize all map loaders
 */
void initMapLoaders();

#endif // ALMOSTTHERE_SRC_GAMEPLAY_MAP_MAPLOADGENERIC_HH
