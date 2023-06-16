#include "AmbientManager.hh"
#include "Particle.hh"
#include "event/Event.hh"
#include "gameplay/base/Game.hh"
#include "gameplay/objs/NoteEvents.hh"

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
}
void AmbientManager::removeEventListeners() const {
  removeEventListener<NoteHitEvent>(noteHitHandler);
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
