#ifndef GAMEPLAY_HUD_BAR
#define GAMEPLAY_HUD_BAR

#include <glm/glm.hpp>

struct DrawContext;

enum BarDirection
{
    HORZ,
    VERT,
};

class Bar
{
public:
    Bar() = default;
    Bar(BarDirection dirIn, const glm::vec2 &pos, int maxLength, int width, bool revertDir = false);
    void setLength(double len); // Set a len value 0-1
    void setColor(glm::vec4 c);
    void draw(DrawContext &ctx);
    void flip();

protected:
    BarDirection dir;
    bool revert;
    bool sharpEnd = false;
    // The begin position
    glm::vec2 pos;
    glm::vec4 color;
    bool vFlip = false;
    int maxl, th; // Max length and thickness
    double len = 0;
};

#endif /* GAMEPLAY_HUD_BAR */
