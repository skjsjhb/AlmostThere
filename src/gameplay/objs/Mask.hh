#ifndef GAMEPLAY_OBJS_MASK
#define GAMEPLAY_OBJS_MASK

// The mask over the camera, for hints
#include "TickObject.hh"
#include "engine/virtual/Graphics.hh"
#include <string>
#include <glm/glm.hpp>

class Mask : TickObject
{
public:
    glm::vec4 color = {1, 0, 0, 1.0};
    float beginRad = 0.8;

    void draw() override;
    void tick() override;
};

#endif /* GAMEPLAY_OBJS_MASK */
