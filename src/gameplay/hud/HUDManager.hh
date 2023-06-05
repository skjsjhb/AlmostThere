#ifndef ALMOSTTHERE_HUDMANAGER_HH
#define ALMOSTTHERE_HUDMANAGER_HH

#include "HPBar.hh"
#include "ShieldBar.hh"
#include "ScoreDisplay.hh"

class Game;
class DrawList;

class HUDManager {
public:
  explicit HUDManager(Game &g) : game(g), hp(g), score(g) {}

  void draw();

protected:
  Game &game;
  HPBar hp;
  ShieldBar shield{};
  ScoreDisplay score;
};

#endif // ALMOSTTHERE_HUDMANAGER_HH
