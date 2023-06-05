#include "TestTools.hh"

#include "gameplay/view/View.hh"
#include "gameplay/base/Game.hh"
#include <memory>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

int main() {
  // Game obj
  Game g;
  // Create a camera looking 'down' towards -z
  View v;
  vec3 e = {0, 0, -1};
  vec3 pos = {0, 0, 1}, d = {0, 0, -1}, u = {1, 0, 0};
  v.screenSize[0] = 100;
  v.screenSize[1] = 100;
  auto c = std::make_shared<Camera>(g);
  v.camera = c;
  v.camera.lock()->setState(pos, d, u, 90.0f, v.screenSize[0] / v.screenSize[1]);

  // When mouse is at the center, ray should be the same as camera direction
  // In this case -z
  vec2 coord = {50, 50};
  auto ray = castMouseRay(v, coord);
  for (int i = 0; i < 3; i++) {
    WANT(ray[i] == e[i])
  }

  // When mouse is at the top center, ray should have a component in +x direction
  // In this case FOV is 90, so it should be x = sqrt(2) / 2 and z = -sqrt(2) / 2
  coord[1] = 0;
  ray = castMouseRay(v, coord);
  vec3 e2 = {float(sqrt(2) / 2), 0, 0};
  e2[2] = -e2[0];
  for (int i = 0; i < 3; i++) {
    WANT(static_cast<int>(ray[i] * 10000) == static_cast<int>(e2[i] * 10000))
  }

  // When mouse is in the top left corner, ray should also have a component in +y direction
  // In this case all 3 axes should have the same length, i.e. sqrt(3) / 3
  coord[0] = 0;
  ray = castMouseRay(v, coord);
  vec3 e3 = {0, 0, float(-sqrt(3) / 3)};
  e3[0] = e3[1] = -e3[2];
  for (int i = 0; i < 3; i++) {
    WANT(static_cast<int>(ray[i] * 10000) == static_cast<int>(e3[i] * 10000))
  }

  // Let's also check the view matrix and projection matrix
  // Since if all above have passed the inv matrix should be OK
  // So we just check it with a simple point and verify that they are inversed
  auto p = v.camera.lock()->getProjectionMatrix();
  auto ivp = v.camera.lock()->getProjectionMatrixInv();
  auto xv = v.camera.lock()->getViewMatrix();
  auto ivv = v.camera.lock()->getViewMatrixInv();
  vec4 o = {0, 0, -1, 0};
  o = glm::normalize(p * xv * o);
  WANT(o[0] == 0 && o[1] == 0)
  auto pd = glm::inverse(p);
  auto vd = glm::inverse(xv);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      WANT(pd[i][j] == ivp[i][j] && vd[i][j] == ivv[i][j])
    }
  }
  TEND
}
