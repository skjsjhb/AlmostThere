#include "View.hh"

glm::vec3 castMouseRay(View &v, const glm::vec2 &coord) {
  glm::vec4 ndc;
  ndc[0] = (2.0f * coord[0]) / v.screenSize[0] - 1.0f;
  ndc[1] = 1 - (2.0f * coord[1]) / v.screenSize[1];
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
