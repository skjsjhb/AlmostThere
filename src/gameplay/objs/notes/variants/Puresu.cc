#include "Puresu.hh"

#include "../NoteTools.hh"
#include "event/Event.hh"
#include "gameplay/base/Game.hh"
#include "gameplay/objs/Note.hh"
#include "gameplay/objs/NoteEvents.hh"
#include "gameplay/score/ScoreValue.hh"
#include "util/Util.hh"

#define PURESU_LENGTH_SCALE 10

void Puresu::performJudge() {
  if (isFake) {
    return;
  }

  auto time = game.mapTimer.getTime();

  auto range = game.rules.judgeTimeWindow.range;
  auto almost = game.rules.judgeTimeWindow.almost;
  auto good = game.rules.judgeTimeWindow.good;
  auto lifeTime = controller->getLifeTime();

  ScoreGrade grade;

  switch (judgeStage) {
  case JUDGED:
  default:isActive = false;
    return;
  case BUSY:
    if (!isPressed()) {
      judgeStage = CLEAR;
    }
    if (!isOverlapped(lifeTime.hitTime, range, time, 0) &&
        time > lifeTime.hitTime) {
      // Lost
      grade = LT;
      judgeStage = JUDGED;
    }
    break;
  case CLEAR:
    // Accepting judge
    if (isOverlapped(lifeTime.hitTime, good, time, 0) &&
        isPressed()) {
      // Turn to active, keep pressing!
      judgeStage = ACTIVE;
    } else {
      if (time > lifeTime.hitTime &&
          !isOverlapped(lifeTime.hitTime, range, time, 0)) {
        // Lost
        grade = LT;
        judgeStage = JUDGED;
      } else if (isPressed()) {
        // Too early
        judgeStage = BUSY;
      }
    }
    break;
  case ACTIVE:
    if (time >= lifeTime.hitTime + lifeTime.length) {
      // Well done!
      grade = PF;
      judgeStage = JUDGED;
      break;
    }
    if (!isPressed()) {
      if (isOverlapped((lifeTime.hitTime + lifeTime.length), almost, time, 0)) {
        // AT
        grade = AT;
        judgeStage = JUDGED;
      } else if (isOverlapped((lifeTime.hitTime + lifeTime.length), good, time, 0)) {
        // AC
        grade = AC;
        judgeStage = JUDGED;
      }
      break;
      // Do not lifeTimeresh time
    } else {
      lastSuccJudge = time; // Refresh time
    }
    if (lastSuccJudge == -1) {
      lastSuccJudge = time; // Reset flag
    }
    if (lastSuccJudge != -1 &&
        time - lastSuccJudge > game.rules.judgeTimeWindow.allowBreak) {
      if (lastSuccJudge - lifeTime.hitTime > 0.5 * lifeTime.length) {
        grade = MD;
      } else {
        // At least you've got to ACTIVE
        grade = TC;
      }
      judgeStage = JUDGED;
    }
  }
  if (judgeStage == JUDGED) {
    game.score.addRecord(NoteScoreEntry::create(typ, grade));
    NoteHitEvent e(game, *this, grade);
    dispatchEvent(e);
  }
}

void Puresu::draw() {
  auto output = controller->getOutput();

  if (!isActive || output.len <= 0) {
    return;
  }

  sizeh /= 2.0; // Temporarily shift
  auto upLen = output.up * float(sizeh);
  auto ctr = output.pos - upLen;

  glm::vec3 pts[4];
  createRect(ctr, output.up, output.norm, sizew, sizeh, pts);

  DrawParam headPar = {
      .shader = "3d/mesh",
      .texture = {"puresu-head"},
      .ctx = game.ctx3D,
  };

  Rect head({pts[0], {0, 1}}, {pts[1], {0, 0}}, {pts[2], {1, 1}}, {pts[3], {1, 0}}, headPar);

  auto scaleFac = output.len * PURESU_LENGTH_SCALE * 0.5;
  auto centerMove = output.up * float(scaleFac);
  auto bCenter = output.pos + centerMove;

  createRect(bCenter, output.up, output.norm, sizew, scaleFac, pts);

  DrawParam bodyPar = {
      .shader = "3d/mesh",
      .texture = {"puresu-body"},
      .ctx = game.ctx3D,
  };

  Rect body({pts[0], {0, 1}}, {pts[1], {0, 0}}, {pts[2], {1, 1}}, {pts[3], {1, 0}}, bodyPar);

  auto tCenter = bCenter + centerMove + upLen;

  createRect(tCenter, output.up, output.norm, sizew, sizeh, pts);
  DrawParam tailPar = {
      .shader = "3d/mesh",
      .texture = {"puresu-tail"},
      .ctx = game.ctx3D,
  };

  Rect tail({pts[0], {0, 1}}, {pts[1], {0, 0}}, {pts[2], {1, 1}}, {pts[3], {1, 0}}, tailPar);

  game.drawList.add(head);
  game.drawList.add(body);
  game.drawList.add(tail);

  sizeh *= 2.0;
}
