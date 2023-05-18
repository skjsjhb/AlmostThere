#ifndef GAMEPLAY_HUD_BAR
#define GAMEPLAY_HUD_BAR

#include <cglm/cglm.h>
#include "engine/virtual/Graphics.hh"

enum BarDirection
{
    HORZ,
    VERT,
};

class Bar
{
public:
    Bar() = default;
    Bar(BarDirection dirIn, vec2 pos, int maxLength, int width, bool revertDir = false);
    void setLength(double len); // Set a len value 0-1
    void setColor(vec4 c);
    void draw(DrawContext &ctx);
    void flip();

protected:
    BarDirection dir;
    bool revert;
    bool sharpEnd = false;
    // The begin position
    vec2 pos;
    vec4 color;
    bool vFlip = false;
    int maxl, th; // Max length and thickness
    double len = 0;
};

#endif /* GAMEPLAY_HUD_BAR */
