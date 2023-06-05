#ifndef GAMEPLAY_VIEW_VIEW
#define GAMEPLAY_VIEW_VIEW

#include <memory>
#include "Camera.hh"
#include <glm/glm.hpp>

struct View {
  std::weak_ptr<Camera> camera;
  glm::vec2 screenSize;
};

glm::vec3 castMouseRay(View &v, const glm::vec2 &coord);

#endif /* GAMEPLAY_VIEW_VIEW */
