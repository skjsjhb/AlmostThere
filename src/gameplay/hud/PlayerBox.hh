#ifndef GAMEPLAY_HUD_PLAYERBOX
#define GAMEPLAY_HUD_PLAYERBOX

#include "gameplay/hud/Bar.hh"

class Player;
struct DrawContext;

class PlayerBox
{
public:
    PlayerBox(Player &p);
    void draw(DrawContext &ctx);

protected:
    double marginLeft, marginTop, width, height;
    Player &player;
    Bar shieldBar, hpBar;
};

#endif /* GAMEPLAY_HUD_PLAYERBOX */
