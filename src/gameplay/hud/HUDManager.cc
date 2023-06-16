#include "HUDManager.hh"

#include "event/Event.hh"
#include "gameplay/base/Game.hh"
#include "gameplay/player/PlayerEvents.hh"
#include <spdlog/spdlog.h>

void HUDManager::draw() {
  // Sync status
  auto &p = game.player;

  auto hpiv = p->getHealthInfo();
  hp.setMaxHP(hpiv.maxHealth);
  hp.setVariant(hpiv.state == HealthState::Normal ? HP_NORMAL : HP_LOW);
  hp.setHP(hpiv.hp, true);

  shield.setShield(hpiv.shield);
  shield.setMaxShield(hpiv.maxShield);
  score.setScore(game.score.exportScore());

  // Draw
  shield.draw();
  hp.draw();
  score.draw();

  emHealth.draw();
  emShield.draw();
}
void HUDManager::addEventListeners() {
  playerDamageEventHandler = addEventListener<PlayerDamageEvent>([this](PlayerDamageEvent &e) -> void {
    if (e.getDamageType() == DamageType::Shield) {
      this->emShield.refresh();
    } else {
      this->emHealth.refresh();
    }
  });
}

void HUDManager::removeEventListeners() const {
  removeEventListener<PlayerDamageEvent>(playerDamageEventHandler);
}
