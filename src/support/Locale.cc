#include "Locale.hh"

#include "Resource.hh"
#include <unordered_map>
#include <fstream>
#include <sstream>
#include "spdlog/spdlog.h"
#include "util/Util.hh"

using namespace spdlog;

static std::unordered_map<std::string, std::string> lcMap;

void setLocale(const std::string &name)
{
    info("Loading locale " + name);
    auto pt = getAppResource("locale/" + name + ".lang");
    std::ifstream f(pt);
    if (f.fail())
    {
        warn("Could not load locale " + name + ", file not readable.");
        return;
    }
    std::stringstream sc;
    sc << f.rdbuf();
    auto lines = splitStr(sc.str(), "\n");
    for (auto &l : lines)
    {
        auto kv = splitStr(l, "=", 2);
        if (kv.size() != 2)
        {

            warn("Invalid language file entry detected. Skipping.");
            continue;
        }
        auto k = kv[0];
        auto v = kv[1];
        trimStr(k);
        trimStr(v);
        lcMap[k] = lcMap[v];
    }
    info("Locale " + name + " loaded.");
}

std::string getLocaleText(const std::string &key)
{
    if (!lcMap.contains(key))
    {
        warn("Missing translation for key: " + key);
        return "";
    }
    return lcMap[key];
}
