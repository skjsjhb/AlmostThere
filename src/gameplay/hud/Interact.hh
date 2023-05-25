#ifndef GAMEPLAY_HUD_INTERACT
#define GAMEPLAY_HUD_INTERACT

#include "engine/virtual/UIHook.hh"
#include <string>

struct DrawContext;
class Player;

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
    // Constructor
    Interact(Player &p);

    // Delete copy constructor
    Interact(const Interact &) = delete;

    // Delete copy assignment constructor
    Interact &operator=(const Interact &) = delete;

    // Destructor
    ~Interact();
    void tick(double absTime);
    void draw(DrawContext &ctx);

protected:
    SkillBtn auxBtn, finalBtn;
    Player &player;
};

#endif /* GAMEPLAY_HUD_INTERACT */
