#ifndef GAMEPLAY_OBJS_SLOT
#define GAMEPLAY_OBJS_SLOT

#include "TickObject.hh"
#include "engine/virtual/Graphics.hh"
#include <cglm/cglm.h>
#include <set>

enum SlotShape
{
    TRIANGLE,
    DIAMOND,
    HEXAGON,
    CIRCLE,
};
// Slot is a shape for notes to drop
class Slot : public TickObject
{
public:
    vec3 center; // Position vector of center point
    vec3 normal; // Normal vector
    vec3 up;     // Up direction
    SlotShape shape;
    void tick(double absTime) override;
    void draw(DrawContext &ctx);

    bool isVisible;
};

#endif /* GAMEPLAY_OBJS_SLOT */
