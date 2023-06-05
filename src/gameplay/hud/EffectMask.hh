#ifndef GAMEPLAY_HUD_EFFECTMASK
#define GAMEPLAY_HUD_EFFECTMASK

#include <string>
#include <utility>

class Game;

class EffectMask {
public:
  EffectMask(Game &g, std::string sd, std::string tx) : game(g), shader(std::move(sd)), texture(std::move(tx)) {};

  void refresh();

  void draw();

protected:
  Game &game;
  std::string shader, texture;
  double beginTime = -1;
};

#endif /* GAMEPLAY_HUD_EFFECTMASK */
