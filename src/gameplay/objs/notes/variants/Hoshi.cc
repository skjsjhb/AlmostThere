#include "Hoshi.hh"

#include "gameplay/base/Game.hh"
#include "util/Util.hh"

void Hoshi::draw() {
  if (!isActive) {
    return;
  }

  /*
             N[0](0,1)
  L[2](1,0) F[4](0,0) B[5](0,0) R[3](1,0)
             S[1](0,1)
  */

  Point points[6]; // NS, LR, FB

  auto output = controller->getOutput();
  auto rightVec = glm::normalize(glm::cross(output.up, output.norm)) * float(sizew);

  auto upVec = output.up * float(sizew);
  auto normVec = output.norm * float(sizew);

  points[0] = {output.pos + normVec, {0, 1}};
  points[1] = {output.pos - normVec, {0, 1}};

  points[2] = {output.pos - rightVec, {1, 0}};
  points[3] = {output.pos + rightVec, {1, 0}};
  points[4] = {output.pos - upVec, {0, 0}};
  points[5] = {output.pos + upVec, {0, 0}};

  DrawParam p = {
      .shader = "3d/mesh",
      .texture = "hoshi",
      .ctx = game.ctx3D,
  };

  TriangleStrip fts(strip(points, {4, 3, 0, 4, 0, 2, 4, 2, 1, 4, 1, 3}), p);
  TriangleStrip bts(strip(points, {5, 2, 0, 5, 0, 3, 5, 3, 1, 5, 1, 2}), p);

  game.drawList.add(fts);
  game.drawList.add(bts);
}
