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

class Slot;

class HitEffect : public TickObject
{
protected:
    double size = 0, initDirection /*, opacity = 1 */;

public:
    double startTime;
    bool isVisible = true;
    Slot *targetSlot;
    void tick(double absTime) override;
    void draw(DrawContext &ctx);
    HitEffect();
};

// Slot is a shape for notes to drop
class Slot : public TickObject
{
public:
    vec3 center; // Position vector of center point
    vec3 normal; // Normal vector
    vec3 up;     // Up direction
    SlotShape shape;
    bool inUse = false; // If in-use, play hit effect.
    void tick(double absTime) override;
    void draw(DrawContext &ctx);

    bool isVisible;

protected:
    double lastGenTime; // Last hit effect played
    std::set<HitEffect *> hitEffects;
};

#endif /* GAMEPLAY_OBJS_SLOT */
