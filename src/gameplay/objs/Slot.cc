#include "Slot.hh"

#include "Note.hh"
#include "NoteDef.hh"
#include "gameplay/base/Game.hh"

#define SLOT_SIZE 1.5

static std::vector<std::string> slotTexName = {"circle", "eureka", "line"};

void Slot::draw() {
  if (!isActive) {
    return;
  }
  auto stat = controller->getOutput();
  glm::vec3 dw;
  dw = glm::normalize(glm::cross(stat.up, stat.norm)) * float(SLOT_SIZE);
  if (variant == LINE) {
    dw *= 1000.0f;
  }
  auto dh = stat.up * float(SLOT_SIZE);
  auto ctr = stat.pos;

  auto rt = ctr + dw + dh;
  auto lt = ctr - dw + dh;
  auto lb = ctr - dw - dh;
  auto rb = ctr + dw - dh;

  Point ltp = {lt, {0, 1}}, lbp = {lb, {0, 0}}, rtp = {rt, {1, 1}}, rbp = {rb, {1, 0}};
  DrawParam p = {.shader = "3d/mesh", .texture = {slotTexName[variant]}, .transparent = true, .ctx = game.ctx3D};
  Rect r = {ltp, lbp, rtp, rbp, p};
  game.drawList.add(r);
}
std::shared_ptr<Slot> Slot::create(SlotVariant va, Game &g, std::shared_ptr<Controller> ct) {
  auto st = std::make_shared<Slot>(g);
  st->controller = std::move(ct);
  st->variant = va;
  return st;
}
