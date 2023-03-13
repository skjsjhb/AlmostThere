#ifndef GAMEPLAY_INPUT_INPUTSET
#define GAMEPLAY_INPUT_INPUTSET

#include <cglm/cglm.h>
#include <set>
#include <list>
#include <map>

class InputSet
{
public:
    void setupInputListeners();
    void pollInputEvents();
    // Keys: only with GLFW
    std::map<int, bool> keyInfo;
    // Touch: only with mobile
    std::set<vec2> touchPoints;
};

#endif /* GAMEPLAY_INPUT_INPUTSET */
