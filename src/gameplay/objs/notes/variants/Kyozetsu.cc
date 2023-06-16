#include "Kyozetsu.hh"

#include "event/Event.hh"
#include "gameplay/base/Game.hh"
#include "gameplay/objs/Note.hh"
#include "gameplay/objs/NoteEvents.hh"
#include "gameplay/score/ScoreValue.hh"
#include "util/Util.hh"

void Kyozetsu::performJudge() {
  if (isFake) {
    return;
  }

  auto lifeTime = controller->getLifeTime();
  auto time = game.mapTimer.getTime();

  ScoreGrade grade;

  switch (judgeStage) {
  case JUDGED:isActive = false;
    return;
  default:
    // In all other cases, check and judge
    if (isOverlapped(lifeTime.hitTime, game.rules.judgeTimeWindow.good, time, 0)) {
      if (isPressed()) {
        // You touched the zone!
        grade = LT;
        judgeStage = JUDGED;
      }
    } else {
      if (time > lifeTime.hitTime + game.rules.judgeTimeWindow.good) {
        // OK very well!
        grade = PF;
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
