#include "AmbientManager.hh"
#include "Particle.hh"
#include "engine/virtual/Audio.hh"
#include "event/Event.hh"
#include "gameplay/base/Game.hh"
#include "gameplay/objs/NoteEvents.hh"
#include "gameplay/player/Player.hh"
#include "gameplay/player/PlayerEvents.hh"
#include "support/Resource.hh"

#include <glm/gtx/rotate_vector.hpp>

#define AMBIENT_PARTICLES_COUNT 4

void AmbientManager::addEventListeners() {
  noteHitHandler = addEventListener<NoteHitEvent>([this](NoteHitEvent &e) -> void {
    auto &st = e.getNote().getControllerConst().getOutput();
    auto time = game.mapTimer.getTime();
    if (e.getGrade() != LT) {
      // Hit effect
      for (int i = 0; i < AMBIENT_PARTICLES_COUNT; ++i) {
        auto dir = glm::rotate(st.up, float(rand()), st.norm); // NOLINT rand is enough
        this->particles.emplace_back(std::make_unique<Particle>(game, st.pos, dir, st.norm, time));
      }
    } else {
      this->particles.emplace_back(std::make_unique<LostParticle>(game, st.pos, st.up, st.norm, time));
    }
  });
  playerDamageHandler = addEventListener<PlayerDamageEvent>([](PlayerDamageEvent &e) -> void {
    if (e.getDamageType() == DamageType::Shield) {
      vtPlaySound(getAppResource("sound/ping.wav"), true);
    } else {
      vtPlaySound(getAppResource("sound/alert.wav"), true);
    }
  });

  playerDieHandler = addEventListener<PlayerDieEvent>([](PlayerDieEvent &e) -> void {
    vtPlaySound(getAppResource("sound/fail.wav"), false);
  });
}
void AmbientManager::removeEventListeners() const {
  removeEventListener<NoteHitEvent>(noteHitHandler);
  removeEventListener<PlayerDamageEvent>(playerDamageHandler);
  removeEventListener<PlayerDieEvent>(playerDieHandler);
}

AmbientManager::~AmbientManager() noexcept {
  removeEventListeners();
}

void AmbientManager::draw() {
  for (auto it = particles.begin(); it != particles.end();) {
    if ((*it)->shouldUnload()) {
      it = particles.erase(it);
    } else {
      ++it;
    }
  }
  for (auto &p : particles) {
    p->draw();
  }
}
