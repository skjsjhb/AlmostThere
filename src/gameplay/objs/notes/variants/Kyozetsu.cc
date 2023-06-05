#include "Kyozetsu.hh"

#include "gameplay/base/Game.hh"
#include "util/Util.hh"

void Kyozetsu::performJudge() {
  if (isFake) {
    return;
  }

  auto &ref = controller->getReference();
  auto time = game.mapTimer.getTime();

  switch (judgeStage) {
  case JUDGED:isActive = false;
    return;
  default:
    // In all other cases, check and judge
    if (isOverlapped(ref.endTime, game.rules.judgeTimeWindow.good, time, 0)) {
      if (isPressed()) {
        // You touched the zone!
        game.score.addRecord(NoteScoreEntry::create(KZTU, LT));
        judgeStage = JUDGED;
      }
    } else {
      if (time > ref.endTime + game.rules.judgeTimeWindow.good) {
        // OK very well!
        game.score.addRecord(NoteScoreEntry::create(KZTU, LT));
        judgeStage = JUDGED;
      }
    }
  }
}
