#include "FlatNote.hh"

#include "NoteTools.hh"
#include "gameplay/base/Game.hh"

void FlatNote::draw() {
  if (!isActive) {
    return;
  }
  auto stat = controller->getState();

  DrawParam p = {
      .shader = "3d/mesh",
      .texture = name,
      .ctx = game.ctx3D,
  };

  glm::vec3 pts[4];
  createRect(stat.pos, stat.up, stat.normal, sizew, sizeh, pts);

  Rect r = {{pts[0], {0, 1}}, {pts[1], {0, 0}}, {pts[2], {1, 1}}, {pts[3], {1, 0}}, p};

  game.drawList.add(std::make_unique<Rect>(r));
}
