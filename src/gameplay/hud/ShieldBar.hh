#ifndef GAMEPLAY_HUD_SHIELDBAR
#define GAMEPLAY_HUD_SHIELDBAR

class Game;

class ShieldBar {
public:
  explicit ShieldBar(unsigned int max = 100, unsigned int init = 100) : sMax(max), sCurrent(init) {};

  void setShield(unsigned int s) { sCurrent = s; }
  void setMaxShield(unsigned int s) { sMax = s; }

  void draw(Game &g) const;

protected:
  unsigned int sMax, sCurrent;
};

#endif /* GAMEPLAY_HUD_SHIELDBAR */
