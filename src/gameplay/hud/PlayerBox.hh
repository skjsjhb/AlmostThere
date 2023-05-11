#ifndef GAMEPLAY_HUD_PLAYERBOX
#define GAMEPLAY_HUD_PLAYERBOX

#include "gameplay/player/Player.hh"
#include "gameplay/hud/Bar.hh"
#include "engine/virtual/Graphics.hh"

class PlayerBox
{
public:
    PlayerBox(Player *p);
    PlayerBox() = default;
    void draw(DrawContext &ctx);

protected:
    double marginLeft, marginTop, width, height;
    Player *playerPtr;
    Bar shieldBar, hpBar;
};

#endif /* GAMEPLAY_HUD_PLAYERBOX */
