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
    RECT,  // For most flat notes
    PRISM, // For Hashi
    OCT,   // For Hoshi
};

struct PolygonShape
{
    // A int pointing which preset to be used, including shaders, etc.
    RenderPreset renderPreset;
    std::string shader, texture;
    std::vector<std::array<float, 3>> points;
    std::map<std::string, float> values;
};

struct DrawContext
{
    Camera cam;
    std::vector<PolygonShape> polygons;
};

extern void vtGraphicsInit();

extern void vtGraphicsCleanUp();

extern void vtDraw(DrawContext &ctx);

extern int vtGetGraphicsError();

#endif /* ENGINE_VIRTUAL_GRAPHICS */
