#ifndef GAMEPLAY_VIEW_VIEW
#define GAMEPLAY_VIEW_VIEW

#include <memory>
#include "Camera.hh"
#include <cglm/cglm.h>

struct View
{
    std::weak_ptr<Camera> camera;
    vec2 screenSize;
};

void castMouseRay(View &v, const vec2 coord, vec3 rayIn);

#endif /* GAMEPLAY_VIEW_VIEW */
