#ifndef GAMEPLAY_CONTROL_CONTROLLER
#define GAMEPLAY_CONTROL_CONTROLLER

#include <vector>
#include <cglm/cglm.h>

#include "gameplay/map/MapDef.hh"
#include <string>
#include <list>

struct ObjTickStatus
{
    vec3 pos, up, normal;
    double len, alpha;
};

class ObjController
{
public:
    void tick(double absTime);

    // Get the three vectors.
    bool shouldLoad(double absTime);
    bool shouldUnload(double absTime);
    void setDep(ObjController *n);

    ObjController() = default; // Fallback
    ObjController(MapObject *ns);

    static void analyzeDepChain(std::list<ObjController *> origin);
    ObjTickStatus currentStatus;

protected:
    MapObject *obj = nullptr;
    ObjController *dep;
};

void initControllerLuaExt();

#endif /* GAMEPLAY_CONTROL_CONTROLLER */
