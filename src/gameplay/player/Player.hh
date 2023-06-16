#ifndef GAMEPLAY_PLAYER_PLAYER
#define GAMEPLAY_PLAYER_PLAYER

class Player;

#include "./chars/CharList.hh"
#include "gameplay/base/Game.hh"
#include <string>
#include <memory>

enum class HealthState {
  Normal,
  Unstable,
  Killed
};

struct HealthInfo {
  unsigned int hp = 0, maxHealth = 0, shield = 0, maxShield = 0;
  HealthState state = HealthState::Normal;
};

class Player {
public:

  explicit Player(Game &g) : game(g) {}

  virtual ~Player() = default;

  [[nodiscard]] HealthInfo getHealthInfo() const { return health; }

  virtual void addEventListeners();

  virtual void removeEventListeners() const;

  virtual void damage(unsigned int amount);

protected:
  Game &game;
  std::string playerName, charName;

  HealthInfo health;

  // Handlers
  unsigned int noteHitHandler = 0;
};

#endif /* GAMEPLAY_PLAYER_PLAYER */
