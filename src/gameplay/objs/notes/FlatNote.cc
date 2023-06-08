#include "FlatNote.hh"

#include "NoteTools.hh"
#include "gameplay/base/Game.hh"

void FlatNote::draw() {
  if (!isActive) {
    return;
  }

  auto output = controller->getOutput();

  DrawParam p = {
      .shader = "3d/mesh",
      .texture = name,
      .ctx = game.ctx3D,
  };

  glm::vec3 pts[4];
  createRect(output.pos, output.up, output.norm, sizew, sizeh, pts);

  Rect r = {{pts[0], {0, 1}}, {pts[1], {0, 0}}, {pts[2], {1, 1}}, {pts[3], {1, 0}}, p};

  game.drawList.add(r);
}
