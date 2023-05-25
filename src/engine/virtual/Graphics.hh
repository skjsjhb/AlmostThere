#ifndef ENGINE_VIRTUAL_GRAPHICS
#define ENGINE_VIRTUAL_GRAPHICS

#include <cglm/cglm.h>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <list>
#include <memory>

#define VT_SD_ARGS 8

enum RenderPreset
{
    RECT,       // For most flat notes
    PRISM_FULL, // For Hashi
    PRISM_BTM,  // Bottom only
    PRISM_HAT,  // Hat only
    PRISM_SIDE, // Side only
    OCT,        // For Hoshi
};

// 3D world polygon
struct Polygon
{
    // A int pointing which preset to be used, including shaders, etc.
    RenderPreset renderPreset;
    bool isOpaque = true;
    std::string shader, texture, subTexture;
    std::vector<std::array<float, 3>> points;
    float args[VT_SD_ARGS] = {0};
};

enum Align
{
    LEFT,  // Or bottom
    RIGHT, // Or top
    CENTER
};

// Text
struct Typography
{
    std::wstring text;
    vec2 pos; // Left bottom
    vec4 color;
    float size = 0.5;
    Align xAlign = LEFT, yAlign = RIGHT;
};

// 2D shapes, currently only rectangle
struct Shape
{
    std::string shader, texture;
    vec2 points[4]; // LT, LB, RT, RB
    float args[VT_SD_ARGS] = {0};
};

class Camera;

struct DrawContext
{
    std::weak_ptr<Camera> cam;
    std::vector<Polygon> polygons;
    std::list<Typography> typos;
    std::list<Shape> shapes;
};

extern void vtGraphicsInit();

extern void vtGraphicsCleanUp();

extern void vtProcessMeshes(DrawContext &ctx);

extern void vtCompleteDraw(DrawContext &ctx);

extern int vtGetGraphicsError();

extern void vtSetBufferSize(int w, int h);

extern void vtSetBackground(const std::string &img);

#endif /* ENGINE_VIRTUAL_GRAPHICS */
