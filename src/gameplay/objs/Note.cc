#include "Note.hh"

#include "engine/virtual/Input.hh"
#include "gameplay/base/Game.hh"
#include "spdlog/spdlog.h"
#include "util/Util.hh"

#include <utility>
#include "notes/NoteTools.hh"

// Note variants
#include "notes/variants/Tapu.hh"
#include "notes/variants/Shizuku.hh"
#include "notes/variants/Hoshi.hh"
#include "notes/variants/Puresu.hh"
#include "notes/variants/Kyozetsu.hh"
#include "notes/variants/Hashi.hh"
#include "notes/variants/Ranku.hh"

using namespace spdlog;

void Note::tick() {
  TickObject::tick();
  performJudge();
}

bool Note::isPressed() {
  auto stat = controller->getOutput();
  if (use3DJudge) {
    return isPressed3D(stat.pos, game.view, game.inputBuf);
  } else {
    glm::vec3 pts[4];
    createRect(stat.pos, stat.up, stat.norm, sizew, sizeh, pts);
    return isPressed2D(pts, game.view, game.inputBuf);
  }
}

std::shared_ptr<Note> Note::create(NoteType tp, Game &g, std::shared_ptr<Controller> ct, bool isFake) {
  std::shared_ptr<Note> a;
  switch (tp) {
  case NoteType::TAPU:a = std::make_shared<Tapu>(g);
    break;
  case NoteType::PRSU:a = std::make_shared<Puresu>(g);
    break;
  case NoteType::SZKU:a = std::make_shared<Shizuku>(g);
    break;
  case NoteType::RKU:a = std::make_shared<Ranku>(g);
    break;
  case NoteType::KZTU:a = std::make_shared<Kyozetsu>(g);
    break;
  case NoteType::HOSHI:a = std::make_shared<Hoshi>(g);
    break;
  case NoteType::HASHI:a = std::make_shared<Hashi>(g);
    break;
  default:warn("Creating note with missing type: " + std::to_string(int(tp)));
    break;
  }
  a->controller = std::move(ct);
  a->isFake = isFake;
  return a;
}
