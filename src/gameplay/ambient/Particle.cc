#include "Particle.hh"

#include "engine/virtual/Graphics.hh"
#include "gameplay/base/Game.hh"
#include "gameplay/objs/notes/NoteTools.hh"

#include <cstdlib>

#define PARTICLE_LIFETIME 0.4
#define PARTICLE_SPEED 2.5
#define PARTICLE_SIZE 0.4

Particle::Particle(Game &g, const glm::vec3 &pos, const glm::vec3 &d, const glm::vec3 &norm, double gt)
    : game(g), initPos(pos), dir(d), normal(norm), genTime(gt) {
  if (rand() % 2 == 0) { // NOLINT 'rand()' is enough
    partTex = "particle/hex";
  } else {
    partTex = "particle/star";
  }
}

static double particleTiming(double x) {
  return 1 - std::pow(1 - x, 2);
}

static double alphaTiming(double x) {
  return std::pow(x, 1.5);
}

bool Particle::shouldUnload() const {
  return game.mapTimer.getTime() - genTime > PARTICLE_LIFETIME;
}

void Particle::draw() {
  auto px = (game.mapTimer.getTime() - genTime) / PARTICLE_LIFETIME;
  auto pct = particleTiming(px);
  auto pos = initPos + dir * float(PARTICLE_SPEED * pct);
  glm::vec3 pts[4];
  createRect(pos, dir, normal, PARTICLE_SIZE, PARTICLE_SIZE, pts);

  auto alpha = float((1 - alphaTiming(px)) * 0.4 + 0.2);
  DrawParam p = {
      .shader = "3d/trans-mesh",
      .texture = {
          partTex
      },
      .transparent = true,
      .args = {alpha},
      .ctx = game.ctx3D,
  };
  Rect r({pts[0], {0, 1}},
         {pts[1], {0, 0}},
         {pts[2], {1, 1}},
         {pts[3], {1, 0}}, p);
  game.drawList.add(r);
}

void LostParticle::draw() {
  auto px = (game.mapTimer.getTime() - genTime) / PARTICLE_LIFETIME;
  auto pct = particleTiming(px);
  auto size = pct * 2.5 + 0.5;
  glm::vec3 pts[4];
  createRect(initPos, dir, normal, size * PARTICLE_SIZE, size * PARTICLE_SIZE, pts);

  auto alpha = float((1 - alphaTiming(px)));
  DrawParam p = {
      .shader = "3d/trans-mesh",
      .texture = {
          "particle/cross"
      },
      .transparent = true,
      .args = {alpha},
      .ctx = game.ctx3D,
  };
  Rect r({pts[0], {0, 1}},
         {pts[1], {0, 0}},
         {pts[2], {1, 1}},
         {pts[3], {1, 0}}, p);
  game.drawList.add(r);
}
