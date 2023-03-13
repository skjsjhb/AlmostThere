#ifndef GAMEPLAY_OBJS_SLOT
#define GAMEPLAY_OBJS_SLOT

#include "TickObject.hh"
#include <cglm/cglm.h>

enum SlotShape
{
    TRIANGLE,
    DIAMOND,
    HEXAGON,
    CIRCLE,
};

// The JudgeLine is actually a Panel.

// Slot is a shape for notes to drop
class Slot : public TickObject
{
public:
    vec3 center; // Position vector of center point
    vec3 normal; // Normal vector
    vec3 up;     // Up direction
    SlotShape shape;
};

// Now panel can just be rect but that can be changed
class Panel : public TickObject
{
protected:
    vec3 basePoint;
    vec3 normal;
};

#endif /* GAMEPLAY_OBJS_SLOT */
