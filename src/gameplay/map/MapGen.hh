/**
 * @brief Map generator module.
 *
 * Map generators are responsible for loading the map file and generate corresponding objects.
 *
 * @author skjsjhb
 */

#ifndef ALMOSTTHERE_SRC_GAMEPLAY_MAP_MAPGEN_HH
#define ALMOSTTHERE_SRC_GAMEPLAY_MAP_MAPGEN_HH

#include <list>
#include <map>
#include <string>
#include <memory>
#include "gameplay/objs/TickObject.hh"

struct MapContent {
  std::list<std::shared_ptr<TickObject>> objects;
  std::map<std::string, std::weak_ptr<TickObject>> nameMap;
};

struct MapMeta {
  std::string id, songName, artist, mapper, chartName, audio, background, banner;
  /**
   * @brief The duration of the map. This is the only value
   * controlling the actual length of the game.
   */
  double duration;
  /**
   * @brief The difficulty level.
   */
  std::string diffLevel, diffColor;
};

struct MapData {
  MapMeta meta;
  MapContent content;
};

#endif // ALMOSTTHERE_SRC_GAMEPLAY_MAP_MAPGEN_HH
