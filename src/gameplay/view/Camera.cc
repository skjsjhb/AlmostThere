#include "Camera.hh"

#include "engine/virtual/Window.hh"
#include <glm/gtc/matrix_transform.hpp>
#include <utility>

#define PERSPECTIVE_NEAR 0.1f
#define PERSPECTIVE_FAR 100.0f

void Camera::setState(glm::vec3 pos_, glm::vec3 direction_, glm::vec3 up_, double fov_, double aspect_) {
  pos = pos_;
  up = up_;
  direction = direction_;
  fov = fov_;
  aspect = aspect_;
  // Calculate and cache matrices
  _cViewMatrix = glm::lookAt(pos, pos + direction, up);
  _cViewMatrixInv = glm::inverse(_cViewMatrix);

  _cProjectionMatrix = glm::perspective(glm::radians(float(fov)), float(aspect), PERSPECTIVE_NEAR, PERSPECTIVE_FAR);
  _cProjectionMatrixInv = glm::inverse(_cProjectionMatrix);
}

void Camera::tick() {
  TickObject::tick();
  auto output = controller->getOutput();
  int wx, wy;
  vtGetWindowSize(wx, wy);

  setState(output.pos, output.norm, output.up, fov, wx / (double) wy);
}

std::shared_ptr<Camera> Camera::create(Game &g, std::shared_ptr<Controller> ct) {
  auto pt = std::make_shared<Camera>(g);
  pt->controller = std::move(ct);
  return pt;
}

glm::mat4 Camera::getViewMatrix() {
  return _cViewMatrix;
}

glm::mat4 Camera::getViewMatrixInv() {
  return _cViewMatrixInv;
}

glm::mat4 Camera::getProjectionMatrix() {
  return _cProjectionMatrix;
}

glm::mat4 Camera::getProjectionMatrixInv() {
  return _cProjectionMatrixInv;
}

glm::vec3 Camera::getPosition() {
  return pos;
}

glm::vec3 Camera::getDir() {
  return direction;
}
