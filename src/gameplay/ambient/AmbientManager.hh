#ifndef ALMOSTTHERE_SRC_GAMEPLAY_AMBIENT_AMBIENTMANAGER_HH
#define ALMOSTTHERE_SRC_GAMEPLAY_AMBIENT_AMBIENTMANAGER_HH

#include "Particle.hh"
#include <list>
#include <memory>

class Game;

/**
 * @brief An AmbientManager manages corresponding ambient effects of a game.
 */
class AmbientManager {
public:
  explicit AmbientManager(Game &g) : game(g) { addEventListeners(); }
  ~AmbientManager() noexcept;
  void addEventListeners();
  void removeEventListeners() const;
  void draw();
protected:
  Game &game;
  std::list<std::unique_ptr<Particle>> particles;
  unsigned int noteHitHandler = 0;
};

#endif // ALMOSTTHERE_SRC_GAMEPLAY_AMBIENT_AMBIENTMANAGER_HH
