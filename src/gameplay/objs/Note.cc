#include "Note.hh"
#include "engine/virtual/Input.hh"
#include "gameplay/base/Game.hh"
#include "spdlog/spdlog.h"
#include "util/Util.hh"

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

std::shared_ptr<Note> Note::createNote(const std::weak_ptr<NoteObject> &obj, Game &g) {
  std::shared_ptr<Note> a;
  auto xobj = obj.lock();
  switch (xobj->noteType) {
  case TAPU:a = std::make_shared<Tapu>(g);
    break;
  case PRSU:a = std::make_shared<Puresu>(g);
    break;
  case SZKU:a = std::make_shared<Shizuku>(g);
    break;
  case RKU:a = std::make_shared<Ranku>(g);
    break;
  case KZTU:a = std::make_shared<Kyozetsu>(g);
    break;
  case HOSHI:a = std::make_shared<Hoshi>(g);
    break;

  case HASHI:a = std::make_shared<Hashi>(g);
    break;

  default:warn("Creating note with missing type: " + std::to_string(xobj->noteType));
    break;
  }
  a->controller = std::make_shared<ObjController>(*xobj);
  a->isFake = obj.lock()->isFake;
  return a;
}

bool Note::isPressed() {
  auto stat = controller->getState();
  if (use3DJudge) {
    return isPressed3D(stat.pos, game.view, game.inputBuf);
  } else {
    glm::vec3 pts[4];
    createRect(stat.pos, stat.up, stat.normal, sizew, sizeh, pts);
    return isPressed2D(pts, game.view, game.inputBuf);
  }
}
