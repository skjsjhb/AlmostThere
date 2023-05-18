#ifndef GAMEPLAY_HUD_INTERACT
#define GAMEPLAY_HUD_INTERACT

#include "engine/virtual/Graphics.hh"
#include "gameplay/player/Player.hh"
#include "engine/virtual/UIHook.hh"
#include <string>

class SkillBtn
{
public:
    float posx, posy, size;
    std::string texture, shader;
    std::string text;
    float blinkScale = 1;
    float stat;
    float textx, texty, tsize;
    void draw(DrawContext &ctx);
    void hook(UIHookXT xt);
    void dehook();

protected:
    unsigned int uhid = 0;
};

// The interaction area based on skills
class Interact
{
public:
    Interact(Player *p);
    Interact() = default;
    ~Interact();
    void tick(double absTime);
    void draw(DrawContext &ctx);

protected:
    SkillBtn auxBtn, finalBtn;
    Player *player;
};

#endif /* GAMEPLAY_HUD_INTERACT */
