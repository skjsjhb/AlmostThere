#ifndef GAMEPLAY_MAP_MAPLOAD
#define GAMEPLAY_MAP_MAPLOAD

#include "MapDef.hh"

/**
 * @brief Loads a game map with specified id.
 *
 * This function searches for the map in default location, read the map file content, and
 * executue the scripts to create an active map object. In past the map object needs to be
 * destructed manually, but now the map will automatically destroy when invalidated.
 *
 * @param mapId The id of the map to load.
 * @return The loaded map object.
 */
GameMap loadMap(const std::string &mapId);

/**
 * @brief Init map load library modules for Lua to load our map.
 *
 * Map loading related functions are defined in map loading module. This function binds them
 * to the global Lua engine.
 */
void initMapLoader();

#endif /* GAMEPLAY_MAP_MAPLOAD */
