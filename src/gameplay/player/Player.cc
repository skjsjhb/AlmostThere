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
  if (health.shield >= amount) {
    health.shield -= amount;
    PlayerDamageEvent de(DamageType::Shield);
    de.dispatch();
    return;
  }
  auto r = amount - health.shield;
  health.shield = 0;

  if (health.hp > r) {
    health.hp -= r;
  } else if (health.state == HealthState::Normal) {
    // Broken core
    health.hp = health.maxHealth;
    health.state = HealthState::Unstable;
  } else {
    // Killed
    health.hp = 0;
    health.state = HealthState::Killed;
    PlayerDieEvent d;
    d.dispatch();
  }
  PlayerDamageEvent e(DamageType::Health);
  e.dispatch();
}
