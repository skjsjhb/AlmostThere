#include "View.hh"

glm::vec3 castMouseRay(View &v, const glm::vec2 &coord) {
  glm::vec4 ndc;
  ndc[0] = (2.0f * coord[0]) / 1600.0f - 1.0f;
  ndc[1] = (2.0f * coord[1]) / 900.0f - 1.0f;
  ndc[2] = -1;
  ndc[3] = 1;

  auto cam = v.camera.lock();
  auto invProj = cam->getProjectionMatrixInv();
  auto invView = cam->getViewMatrixInv();

  auto eyeRay = invProj * ndc;
  eyeRay[2] = -1;
  eyeRay[3] = 0;

  auto fRay = invView * eyeRay;
  return glm::normalize(fRay);
}
