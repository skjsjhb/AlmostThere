#include "TickObject.hh"

#include "gameplay/base/Game.hh"

bool TickObject::shouldTick() const
{
    auto t = game.mapTimer.getTime();
    auto ref = controller->getReference();
    return t >= ref.genTime && t <= ref.endTime + ref.length + OBJ_UNLOAD_DELAY;
}

void TickObject::tick()
{
    controller->tick(game.mapTimer.getTime());
}
