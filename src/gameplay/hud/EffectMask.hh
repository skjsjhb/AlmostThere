#ifndef GAMEPLAY_HUD_EFFECTMASK
#define GAMEPLAY_HUD_EFFECTMASK

#include <string>

class Game;

class EffectMask
{
public:
    EffectMask(Game &g, const std::string &sd, const std::string &tx) : game(g), shader(sd), texture(tx){};
    void refresh();
    void draw();

protected:
    Game &game;
    std::string shader, texture;
    double beginTime = -1;
};

#endif /* GAMEPLAY_HUD_EFFECTMASK */
