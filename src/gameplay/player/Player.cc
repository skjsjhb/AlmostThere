#include "Player.hh"

#include "PlayerEvents.hh"
#include "gameplay/objs/NoteEvents.hh"
#include "spdlog/spdlog.h"

using namespace spdlog;

void Player::addEventListeners() {
  noteHitHandler = addEventListener<NoteHitEvent>([this](NoteHitEvent &e) -> void {
    if (e.getGrade() == LT) {
      // Damage
      this->damage(5);
    }
  });
}

void Player::removeEventListeners() const {
  removeEventListener<NoteHitEvent>(noteHitHandler);
}

void Player::damage(unsigned int amount) {
  if (health.state == HealthState::Killed) {
    return;
  }
  if (health.shield >= amount) {
    health.shield -= amount;
    if (health.shield == 0) {
      PlayerShieldBreakEvent e(*this);
      e.dispatch();
    }
    PlayerDamageEvent de(*this, DamageType::Shield);
    de.dispatch();
    return;
  }
  auto r = amount - health.shield;
  if (health.shield > 0) {
    PlayerShieldBreakEvent e(*this);
    e.dispatch();
  }
  health.shield = 0;

  if (health.hp > r) {
    health.hp -= r;
  } else if (health.state == HealthState::Normal) {
    // Broken core
    PlayerCoreUnstableEvent e(*this);
    e.dispatch();
    health.hp = health.maxHealth;
    health.state = HealthState::Unstable;
  } else {
    // Killed
    health.hp = 0;
    health.state = HealthState::Killed;
    PlayerDieEvent d(*this);
    d.dispatch();
  }
  PlayerDamageEvent e(*this, DamageType::Health);
  e.dispatch();
}
