#include "Hashi.hh"

#include "gameplay/base/Game.hh"
#include "util/Util.hh"
#include <glm/gtx/rotate_vector.hpp>

#define HASHI_LENGTH_SCALE 10.0

void Hashi::performJudge() {
  if (isFake) {
    return;
  }
  auto time = game.mapTimer.getTime();

  auto ref = controller->getLifeTime();

  switch (judgeStage) {
  case JUDGED:isActive = false;
    return;
  default:
    if (time > ref.hitTime && time < ref.hitTime + ref.length) {
      if (lastSuccJudge == -1) {
        lastSuccJudge = time;
        return;
      }
      if (isPressed()) {
        judgedLength += (time - lastSuccJudge);
      }
      lastSuccJudge = time;
    } else if (time > ref.hitTime + ref.length) {
      judgeStage = JUDGED;
      auto comRate = judgedLength / ref.length;
      if (comRate > 0.95) {
        game.score.addRecord(NoteScoreEntry::create(typ, PF));
      } else if (comRate > 0.85) {
        game.score.addRecord(NoteScoreEntry::create(typ, AT));
      } else if (comRate > 0.70) {
        game.score.addRecord(NoteScoreEntry::create(typ, AC));
      } else if (comRate > 0.45) {
        game.score.addRecord(NoteScoreEntry::create(typ, MD));
      } else if (comRate > 0) {
        game.score.addRecord(NoteScoreEntry::create(typ, TC));
      } else {
        game.score.addRecord(NoteScoreEntry::create(typ, LT));
      }
    }
  }
}

void Hashi::draw() {
  auto output = controller->getOutput();
  if (!isActive || output.len <= 0) {
    return;
  }
  auto right = glm::normalize(glm::cross(output.up, output.norm)) * float(sizew);

  Point btmPoints[6], headPoints[6];

  /*
  BOTTOM:
    2  1
  3      0
    4  5
  */

  for (int i = 0; i < 6; i++) {
    auto ang = glm::radians(float(i) * 60.0f);
    btmPoints[i] = {
        output.pos + glm::rotate(right, ang, output.norm), // Pos Coord
        glm::vec2(0.5, 0.5) + glm::rotate(glm::vec2({0.5, 0}), ang),
    };
  }

  auto upLength = glm::normalize(output.norm) * float(output.len * HASHI_LENGTH_SCALE);

  // In the past, long Hashi notes requires cutting in order to render them
  // correctly. Now Hashi are opaque and there is no need to calculate this
  // anymore.
  for (int i = 0; i < 6; ++i) {
    // Get corresponding point on the head face
    auto b = btmPoints[i];
    headPoints[i] = {b.getPosition() + upLength, b.getTexCoord()};
  }

  DrawParam hat = {
      .shader = "3d/mesh",
      .texture = "hashi-hat",
      .ctx = game.ctx3D,
  };
  std::vector<unsigned int> ptInd = {5, 0, 1, 5, 1, 2, 5, 2, 3, 5, 3, 4};
  TriangleStrip tsb = {strip(btmPoints, ptInd), hat};
  TriangleStrip tsp = {strip(headPoints, ptInd), hat};

  game.drawList.add(std::make_unique<TriangleStrip>(tsb));
  game.drawList.add(std::make_unique<TriangleStrip>(tsp));

  DrawParam side = {
      .shader = "3d/mesh",
      .texture = "hashi-side",
      .ctx = game.ctx3D,
  };

  for (int i = 0; i < 6; ++i) {
    Rect r(
        {headPoints[i].getPosition(), {0, 1}},
        {btmPoints[i].getPosition(), {0, 0}},
        {headPoints[i + 1].getPosition(), {1, 1}},
        {btmPoints[i + 1].getPosition(), {1, 0}},
        side);
    game.drawList.add(std::make_unique<Rect>(r));
  }
}
