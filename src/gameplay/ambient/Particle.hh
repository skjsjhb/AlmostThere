#ifndef GAMEPLAY_AMBIENT_PARTICLE
#define GAMEPLAY_AMBIENT_PARTICLE

#include <string>
#include <glm/glm.hpp>

class DrawList;
class Game;

class Particle {
public:
  Particle(Game &game, const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &norm, double genTime);

  [[nodiscard]] bool shouldUnload() const;

  virtual void draw();

  virtual ~Particle() = default;

protected:
  Game &game;
  glm::vec3 initPos, dir, normal;
  double genTime;
  std::string partTex;
};

class LostParticle : public Particle {
public:
  using Particle::Particle;
  void draw() override;
};

#endif /* GAMEPLAY_AMBIENT_PARTICLE */
