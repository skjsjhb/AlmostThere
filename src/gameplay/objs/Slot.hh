#ifndef GAMEPLAY_OBJS_SLOT
#define GAMEPLAY_OBJS_SLOT

#include "TickObject.hh"
#include "engine/virtual/Graphics.hh"
#include "gameplay/control/Controller.hh"
#include "gameplay/map/MapDef.hh"
#include <memory>
#include <set>

// Slot is a shape for notes to drop
class Slot : public TickObject
{
public:
    SlotVariant variant;
    void draw();

    // Constructor
    using TickObject::TickObject;
    bool isActive = true;

    static std::shared_ptr<Slot> createSlot(std::weak_ptr<SlotObject> o, Game &g);
};

#endif /* GAMEPLAY_OBJS_SLOT */
