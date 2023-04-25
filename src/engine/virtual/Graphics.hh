#ifndef ENGINE_VIRTUAL_GRAPHICS
#define ENGINE_VIRTUAL_GRAPHICS

#include <cglm/cglm.h>
#include <vector>
#include <array>
#include "gameplay/objs/World.hh"
#include <string>
#include <map>

enum RenderPreset
{
    RECT,       // For most flat notes
    PRISM_FULL, // For Hashi
    PRISM_BTM,  // Bottom only
    PRISM_HAT,  // Hat only
    PRISM_SIDE, // Side only
    OCT,        // For Hoshi
};

struct PolygonShape
{
    // A int pointing which preset to be used, including shaders, etc.
    RenderPreset renderPreset;
    bool isOpaque = true;
    std::string shader, texture, subTexture;
    std::vector<std::array<float, 3>> points;
    std::map<std::string, float> values;
    std::map<std::string, std::array<float, 4>> valuesVec4;
};

struct DrawContext
{
    Camera cam;
    std::vector<PolygonShape> polygons;
};

extern void vtGraphicsInit();

extern void vtGraphicsCleanUp();

extern void vtDraw(DrawContext &ctx);

extern void vtFinalizeDraw(DrawContext &ctx);

extern int vtGetGraphicsError();

#endif /* ENGINE_VIRTUAL_GRAPHICS */
