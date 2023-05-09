#ifndef GAMEPLAY_OBJS_SLOT
#define GAMEPLAY_OBJS_SLOT

#include "TickObject.hh"
#include "engine/virtual/Graphics.hh"
#include "gameplay/control/Controller.hh"
#include <cglm/cglm.h>
#include <set>

// Slot is a shape for notes to drop
class Slot : public TickObject
{
public:
    vec3 center; // Position vector of center point
    vec3 normal; // Normal vector
    vec3 up;     // Up direction
    SlotVariant variant;
    void tick(double absTime) override;
    void draw(DrawContext &ctx);

    bool isVisible = true;
};

#endif /* GAMEPLAY_OBJS_SLOT */
