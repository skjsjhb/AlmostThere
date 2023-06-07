#include "TickObject.hh"

#include "gameplay/base/Game.hh"

bool TickObject::shouldTick() const {
  auto t = game.mapTimer.getTime();
  auto ref = controller->getLifeTime();
  return t >= ref.genTime && t <= ref.unloadTime;
}

void TickObject::tick() {
  controller->tick(game.mapTimer.getTime());
}
