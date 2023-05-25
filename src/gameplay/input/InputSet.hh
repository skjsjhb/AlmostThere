#ifndef GAMEPLAY_INPUT_INPUTSET
#define GAMEPLAY_INPUT_INPUTSET

#include <cglm/cglm.h>
#include <set>
#include <list>
#include <map>
#include <array>

/**
 * @deprecated Use input buffer built-in in the engine spec.
 */
class InputSet
{
public:
    void setupInputListeners();
    void pollInputEvents();
    std::set<std::array<float, 2>> touchPoints;
};

#endif /* GAMEPLAY_INPUT_INPUTSET */
