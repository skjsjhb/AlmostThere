#ifndef GAMEPLAY_HUD_SHIELDBAR
#define GAMEPLAY_HUD_SHIELDBAR

#include "EffectMask.hh"

class Game;

class ShieldBar {
public:
  explicit ShieldBar(Game &g, unsigned int max = 100, unsigned int init = 100) :
      game(g), sMax(max), sCurrent(init) {};

  void setShield(unsigned int s) { sCurrent = s; }
  void setMaxShield(unsigned int s) { sMax = s; }

  void draw() const;

protected:
  Game &game;
  unsigned int sMax, sCurrent;
};

#endif /* GAMEPLAY_HUD_SHIELDBAR */
