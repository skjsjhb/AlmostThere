#ifndef GAMEPLAY_OBJS_SLOT
#define GAMEPLAY_OBJS_SLOT

#include "TickObject.hh"
#include "engine/virtual/Graphics.hh"
#include "NoteDef.hh"
#include <memory>
#include <set>

// Slot is a shape for notes to drop
class Slot : public TickObject {
public:
  SlotVariant variant;

  void draw() override;

  // Constructor
  using TickObject::TickObject;
  bool isActive = true;

  static std::shared_ptr<Slot> create(SlotVariant va, Game &g, std::shared_ptr<Controller> ct);
};

#endif /* GAMEPLAY_OBJS_SLOT */
