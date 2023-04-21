#ifndef GAMEPLAY_OBJS_MASK
#define GAMEPLAY_OBJS_MASK

// The mask over the camera, for hints
#include "TickObject.hh"
#include <cglm/cglm.h>
#include "engine/virtual/Graphics.hh"
#include <string>

class Mask : TickObject
{
public:
    vec4 color = {1, 0, 0, 1.0};
    float beginRad = 0.8;

    void draw(DrawContext &ctx);
    void tick(double absTime) override;
};

#endif /* GAMEPLAY_OBJS_MASK */
