#ifndef GAMEPLAY_OBJS_SLOT
#define GAMEPLAY_OBJS_SLOT

#include "TickObject.hh"
#include "engine/virtual/Graphics.hh"
#include "gameplay/control/Controller.hh"
#include "gameplay/map/MapDef.hh"
#include <memory>
#include <cglm/cglm.h>
#include <set>

// Slot is a shape for notes to drop
class Slot : public TickObject
{
public:
    SlotVariant variant;
    void draw(DrawContext &ctx);
    bool isActive = true;

    static std::shared_ptr<Slot> createSlot(std::weak_ptr<SlotObject> o);
};

#endif /* GAMEPLAY_OBJS_SLOT */
