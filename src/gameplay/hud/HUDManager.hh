#ifndef ALMOSTTHERE_HUDMANAGER_HH
#define ALMOSTTHERE_HUDMANAGER_HH

#include "HPBar.hh"
#include "ShieldBar.hh"
#include "ScoreDisplay.hh"
#include "gameplay/hud/EffectMask.hh"

class Game;
class DrawList;

class HUDManager {
public:
  explicit HUDManager(Game &g) : game(g), hp(g), shield(g), score(g),
                                 emShield(g, "hud/effect-mask", "hud/shield-effect"),
                                 emHealth(g, "hud/effect-mask", "hud/blood-effect") {
    addEventListeners();
  }

  void draw();

  void addEventListeners();
  void removeEventListeners() const;

  ~HUDManager() noexcept { removeEventListeners(); };

protected:
  Game &game;
  HPBar hp;
  ShieldBar shield;
  ScoreDisplay score;
  EffectMask emShield, emHealth;
  unsigned int playerDamageEventHandler = 0;
};

#endif // ALMOSTTHERE_HUDMANAGER_HH
