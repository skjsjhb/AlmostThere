#ifndef SUPPORT_RESOURCE
#define SUPPORT_RESOURCE

#include <string>
#include <vector>

void setAppResourceRoot(const std::string &newRoot);

std::string getMapResource(const std::string &mapId, const std::string &fileName);

std::string getAppResource(const std::string &fileName);

std::vector<std::string> getDirFiles(const std::string &dirName);

#endif /* SUPPORT_RESOURCE */
