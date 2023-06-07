#include "CatchNote.hh"

#include "NoteTools.hh"
#include "gameplay/base/Game.hh"
#include "util/Util.hh"

#define CATCH_NOTE_WINDOW 0.005

void CatchNote::performJudge() {
  if (isFake) {
    return;
  }
  auto time = game.mapTimer.getTime();
  auto lifeTime = controller->getLifeTime();

  switch (judgeStage) {
  case JUDGED:isActive = false;
    return;
  default:
    // In all other cases, check and judge
    if (isOverlapped(lifeTime.hitTime, CATCH_NOTE_WINDOW, time, 0)) {
      if (isPressed()) {
        // You got it
        game.score.addRecord(NoteScoreEntry::create(typ, PF));
        judgeStage = JUDGED;
      }
    } else {
      if (time > lifeTime.hitTime) {
        // You failed!
        game.score.addRecord(NoteScoreEntry::create(typ, LT));
        judgeStage = JUDGED;
      }
    }
  }
}
