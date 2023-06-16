#ifndef GAMEPLAY_PLAYER_CHARS_DEFAULT
#define GAMEPLAY_PLAYER_CHARS_DEFAULT

#include "../Player.hh"

class DefaultChar : public Player {
public:
  explicit DefaultChar(Game &g) : Player(g) {
    health = {
        .hp = 20,
        .maxHealth = 20,
        .shield = 30,
        .maxShield =30,
        .state = HealthState::Normal
    };
  }

};

#endif /* GAMEPLAY_PLAYER_CHARS_DEFAULT */
