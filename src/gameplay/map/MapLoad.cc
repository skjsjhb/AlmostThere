#include "MapLoad.hh"

#include "./luamap/LuaMapGen.hh"
#include "support/Resource.hh"
#include "util/Util.hh"
#include <spdlog/spdlog.h>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

#define LOADIV_FILE_NAME "loadiv"

using MapLoader = std::function<MapData(const std::string &, Game &)>;

static std::map<std::string, MapLoader> LOADERS = {
    {"lua", generateLuaMap}
};

MapData nilLoader(const std::string &, Game &) {
  return {};
}

static MapLoader getMapLoader(const std::string &mapId) {
  auto pt = getMapResource(mapId, LOADIV_FILE_NAME);
  std::stringstream content;
  std::ifstream ivFile(pt);
  if (!ivFile.fail()) {
    content << ivFile.rdbuf();
    auto fst = splitStr(content.str(), " ");
    for (auto &f : fst) {
      for (auto &p : LOADERS) {
        if (f == p.first) {
          auto iv = std::string("Map loader '").append(f).append("' detected for map '").append(mapId).append("'");
          spdlog::info(iv);
          return p.second;
        }
      }
    }
  }
  spdlog::error("No loader found for map " + mapId);
  return nilLoader;
}

MapData loadMapEx(const std::string &mid, Game &g) {
  return getMapLoader(mid)(mid, g);
}

void initMapLoaders() {
  initLuaMapLoader();
}
