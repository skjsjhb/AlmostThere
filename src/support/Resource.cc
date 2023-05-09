#include "Resource.hh"
#include <filesystem>

#define MAP_ROOT_NAME "maps"
#define ASSET_ROOT_NAME "assets"

static std::string appResRoot = ".";

void setAppResourceRoot(const std::string &newRoot)
{
    appResRoot = newRoot;
}

std::string getMapResource(const std::string &mapId, const std::string &fileName)
{
    return appResRoot + "/" + MAP_ROOT_NAME + "/" + mapId + "/" + fileName;
}

std::string getAppResource(const std::string &fileName)
{
    return appResRoot + "/" + ASSET_ROOT_NAME + "/" + fileName;
}

std::vector<std::string> getDirFiles(const std::string &dirName)
{
    std::vector<std::string> output;
    for (auto &entry : std::filesystem::directory_iterator(dirName))
    {
        output.push_back(entry.path().string());
    }
    return output;
}