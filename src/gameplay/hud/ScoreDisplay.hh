#ifndef GAMEPLAY_HUD_SCOREDISPLAY
#define GAMEPLAY_HUD_SCOREDISPLAY

class Game;

class ScoreDisplay {
public:
  explicit ScoreDisplay(Game &g) : game(g) {};

  void setScore(unsigned int s);

  void draw();

protected:
  Game &game;
  unsigned int targetScore = 0, displayScore = 0, beginScore = 0;
  double beginTime = -1;
};

#endif /* GAMEPLAY_HUD_SCOREDISPLAY */
