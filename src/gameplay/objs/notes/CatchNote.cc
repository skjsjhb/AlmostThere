#include "CatchNote.hh"

#include "NoteTools.hh"
#include "gameplay/base/Game.hh"
#include "util/Util.hh"
#include "gameplay/objs/NoteEvents.hh"

#define CATCH_NOTE_WINDOW 0.005

void CatchNote::performJudge() {
  if (isFake) {
    return;
  }
  auto time = game.mapTimer.getTime();
  auto lifeTime = controller->getLifeTime();

  ScoreGrade grade;
  switch (judgeStage) {
  case JUDGED:isActive = false;
    return;
  default:
    // In all other cases, check and judge
    if (isOverlapped(lifeTime.hitTime, CATCH_NOTE_WINDOW, time, 0)) {
      if (isPressed()) {
        // You got it
        grade = PF;
        judgeStage = JUDGED;
      }
    } else {
      if (time > lifeTime.hitTime) {
        // You failed!
        grade = LT;
        judgeStage = JUDGED;
      }
    }
  }

  if (judgeStage == JUDGED) {
    game.score.addRecord(NoteScoreEntry::create(typ, grade));
    NoteHitEvent e(game, *this, grade);
    dispatchEvent(e);
  }
}
