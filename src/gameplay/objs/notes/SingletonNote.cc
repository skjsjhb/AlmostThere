#include "SingletonNote.hh"

#include "gameplay/base/Game.hh"
#include "gameplay/score/ScoreValue.hh"

#include "util/Util.hh"
#include "NoteTools.hh"

void SingletonNote::performJudge()
{
    auto time = game.mapTimer.getTime();
    if (isFake)
    {
        // Fake notes are not judged
        return;
    }
    auto range = game.rules.judgeTimeWindow.range;

    auto &ref = controller->getReference();

    switch (judgeStage)
    {
    case BUSY:
        // We want the player to actually 'tap' the note, so it must be released first.

        // If it's already released, change its status.
        if (!isPressed())
        {
            judgeStage = CLEAR;
        }
        // If it's too late...
        if (time > ref.endTime + range)
        {
            game.score.addRecord(NoteScoreEntry::create(typ, LT));
            judgeStage = JUDGED;
        }
        break;
    case CLEAR:
        // The note position is cleared, but the note is not in its judge window yet.
        // If it went into its window and can be judged, activate it.
        if (isOverlapped(ref.endTime, range, time, 0))
        {
            judgeStage = ACTIVE;
        }
        else
        {
            // If it's already too late...
            if (time > ref.endTime + range)
            {
                game.score.addRecord(NoteScoreEntry::create(typ, LT));
                judgeStage = JUDGED;
            }
            // If the position is pressed again, but still too early, then reset it to BUSY.
            else if (isPressed())
            {
                // Too early
                judgeStage = BUSY;
            }
        }
        break;
    case ACTIVE:
        // Now in judge window
        if (isPressed())
        {
            judgeStage = JUDGED;
            auto scr = getTimingGrade(time, ref.endTime, game.rules);
            game.score.addRecord(NoteScoreEntry::create(typ, scr));
        }
        else if (!isOverlapped(ref.endTime, range, time, 0))
        {
            // Already too late. Ready to lost
            judgeStage = CLEAR;
        }
        break;
    case JUDGED:
    default:
        // End judge
        // TODO: play effect
        isActive = false;
        return;
    }
}
