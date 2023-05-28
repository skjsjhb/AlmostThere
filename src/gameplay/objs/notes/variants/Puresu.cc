#include "Puresu.hh"

#include "../NoteTools.hh"
#include "gameplay/base/Game.hh"
#include "util/Util.hh"
#include <glm/glm.hpp>

#define PURESU_LENGTH_SCALE 10

void Puresu::performJudge()
{
    if (isFake)
    {
        return;
    }

    auto time = game.mapTimer.getTime();

    auto range = game.rules.judgeTime.range;
    auto almost = game.rules.judgeTime.almost;
    auto good = game.rules.judgeTime.good;

    auto &ref = controller->getReference();

    switch (judgeStage)
    {
    case JUDGED:
    default:
        isActive = false;
        return;
    case BUSY:
        if (!isPressed())
        {
            judgeStage = CLEAR;
        }
        if (!isOverlapped(ref.endTime, range, time, 0) &&
            time > ref.endTime)
        {
            // Lost
            game.score.addRecord(NoteScoreEntry::create(PRSU, LT));
            judgeStage = JUDGED;
        }
        break;
    case CLEAR:
        // Accepting judge
        if (isOverlapped(ref.endTime, good, time, 0) &&
            isPressed())
        {
            // Turn to active, keep pressing!
            judgeStage = ACTIVE;
        }
        else
        {
            if (time > ref.endTime &&
                !isOverlapped(ref.endTime, range, time, 0))
            {
                // Lost
                game.score.addRecord(NoteScoreEntry::create(PRSU, LT));
                judgeStage = JUDGED;
            }
            else if (isPressed())
            {
                // Too early
                judgeStage = BUSY;
            }
        }
        break;
    case ACTIVE:
        if (time >= ref.endTime + ref.length)
        {
            // Well done!
            game.score.addRecord(NoteScoreEntry::create(PRSU, PF));
            judgeStage = JUDGED;
            return;
        }
        if (!isPressed())
        {
            if (isOverlapped((ref.endTime + ref.length), almost, time, 0))
            {
                // AT
                game.score.addRecord(NoteScoreEntry::create(PRSU, AT));
                judgeStage = JUDGED;
                return;
            }
            else if (isOverlapped((ref.endTime + ref.length), good, time, 0))
            {
                // AC
                game.score.addRecord(NoteScoreEntry::create(PRSU, AC));
                judgeStage = JUDGED;
                return;
            }
            // Do not refresh time
        }
        else
        {
            lastSuccJudge = time; // Refresh time
        }
        if (lastSuccJudge == -1)
        {
            lastSuccJudge = time; // Reset flag
        }
        if (lastSuccJudge != -1 &&
            time - lastSuccJudge > game.rules.judgeTime.allowBreak)
        {
            if (lastSuccJudge - ref.endTime > 0.5 * ref.length)
            {
                game.score.addRecord(NoteScoreEntry::create(PRSU, MD));
            }
            else
            {
                // At least you've got to ACTIVE
                game.score.addRecord(NoteScoreEntry::create(PRSU, TC));
            }
            judgeStage = JUDGED;
        }

        break;
    }
}

void Puresu::draw()
{
    auto stat = controller->getState();

    if (!isActive || stat.len <= 0)
    {
        return;
    }

    Polygon head, tail, body;
    sizeh /= 2.0; // Temporarily shift
    auto upLen = stat.up * float(sizeh);
    auto ctr = stat.pos - upLen;

    glm::vec3 pts[4];
    createRect(ctr, stat.up, stat.normal, sizew, sizeh, pts);

    head.renderPreset = RECT;
    head.shader = "3d/rect";
    head.texture = "puresu-head";
    head.points = {pts[0], pts[1], pts[2], pts[3]};

    auto scaleFac = stat.len * PURESU_LENGTH_SCALE * 0.5;
    auto centerMove = stat.up * float(scaleFac);
    auto bCenter = stat.pos + centerMove;

    createRect(bCenter, stat.up, stat.normal, sizew, scaleFac, pts);

    body.renderPreset = RECT;
    body.shader = "3d/rect";
    body.texture = "puresu-body";
    body.points = {pts[0], pts[1], pts[2], pts[3]};

    auto tCenter = bCenter + centerMove + upLen;

    createRect(tCenter, stat.up, stat.normal, sizew, sizeh, pts);
    tail.renderPreset = RECT;
    tail.shader = "3d/rect";
    tail.texture = "puresu-tail";
    tail.points = {pts[0], pts[1], pts[2], pts[3]};

    game.drawContext.polygons.push_back(body);
    game.drawContext.polygons.push_back(tail);
    game.drawContext.polygons.push_back(head);

    sizeh *= 2.0;
}
