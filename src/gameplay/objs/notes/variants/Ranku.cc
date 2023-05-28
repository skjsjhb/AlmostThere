#include "Ranku.hh"

#include "../NoteTools.hh"
#include "gameplay/base/Game.hh"
#include "util/Util.hh"

#define CATCH_NOTE_WINDOW 0.005

void Ranku::performJudge()
{
    if (isFake)
    {
        return;
    }
    auto time = game.mapTimer.getTime();
    auto &ref = controller->getReference();
    auto stat = controller->getState();

    switch (judgeStage)
    {
    case JUDGED:
        isActive = false;
        return;
    default:
        // In all other cases, check and judge
        if (isOverlapped(ref.endTime, CATCH_NOTE_WINDOW, time, 0))
        {
            if (isPressed())
            {
                // You got it
                // TODO: space judge
                game.score.addRecord(NoteScoreEntry::create(typ, PF));
                judgeStage = JUDGED;
            }
        }
        else
        {
            if (time > ref.endTime)
            {
                // You failed!
                game.score.addRecord(NoteScoreEntry::create(typ, LT));
                judgeStage = JUDGED;
            }
        }
    }
}
