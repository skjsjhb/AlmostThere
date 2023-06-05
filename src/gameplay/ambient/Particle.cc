#include "Particle.hh"

#include "engine/virtual/Graphics.hh"

Particle::Particle(const glm::vec3 &p, const glm::vec3 &v, const glm::vec3 &g, const glm::vec3 &n, double gt,
                   double lt) {
  initPos = p;
  initV = v;
  gravity = g;
  normal = n;
  genTime = gt;
  lifeTime = lt;
}

bool Particle::tick(double absTime) {
  // dx = vt + 0.5 * a * t^2
  // dv = at
  float t = absTime - genTime;
  if (t > lifeTime) {
    return false;
  }
  currentPos = initPos + initV * t + gravity * t * t * 0.5f;
  currentV = initV + gravity * t;
}

void Particle::draw(DrawList &d) {
  auto iUp = glm::normalize(currentV);
}
