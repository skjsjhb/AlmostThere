#include "Note.hh"
#include "util/Util.hh"

#include <array>

#define BASE_FALL_SPEED 10.0
#define FLAT_NOTE_SIZE 1.0
#define HOSHI_NOTE_SIZE 0.8
#define HASHI_NOTE_SIZE 0.6
#define ASSIST_RING_SIZE 0.6
#define HASHI_CUT_THRESHOLD 1.0
#define ASSIST_RING_TIME 1.0

#define NOTE_FLOAT_THRESHOLD 0.01
#define SHIZUKU_JUDGE_WINDOW 0.01

void AbstractNote::tick(double absTime)
{
    if (!isVisible)
    {
        return;
    }
    // TODO: Obtain fall speed from chart
    auto scale = (hitTime - absTime) * BASE_FALL_SPEED;
    vec3 offset;
    glm_vec3_scale(targetSlot->up, scale, offset);
    glm_vec3_add(targetSlot->center, offset, basePosition);
}

void mkRectPoints(PolygonShape &pg, vec3 center, vec3 upLen, vec3 rightLen)
{
    vec3 lt, rt, lb, rb, t;
    glm_vec3_add(center, rightLen, t);
    glm_vec3_add(t, upLen, rt);
    glm_vec3_sub(t, upLen, rb);
    glm_vec3_sub(center, rightLen, t);
    glm_vec3_add(t, upLen, lt);
    glm_vec3_sub(t, upLen, lb);
    pg.points.push_back(std::to_array(lt));
    pg.points.push_back(std::to_array(lb));
    pg.points.push_back(std::to_array(rt));
    pg.points.push_back(std::to_array(rb));
}

static void commonRectDraw(DrawContext &ctx, const std::string sdName, AbstractNote *obj)
{
    if (!obj->isVisible)
    {
        return;
    }
    vec3 right;
    vec3 dw, dh;
    glm_vec3_cross(obj->targetSlot->up, obj->targetSlot->normal, right);
    glm_vec3_normalize(right);
    glm_vec3_scale(right, FLAT_NOTE_SIZE, dw);
    glm_vec3_scale(obj->targetSlot->up, FLAT_NOTE_SIZE, dh);

    PolygonShape pg;
    mkRectPoints(pg, obj->basePosition, dh, dw);
    pg.renderPreset = RECT;
    pg.shader = "rect";
    pg.texture = sdName;
    ctx.polygons.push_back(pg);
}

void Tapu::draw(DrawContext &ctx)
{
    commonRectDraw(ctx, "tapu", this);
}

void Shizuku::draw(DrawContext &ctx)
{
    commonRectDraw(ctx, "shizuku", this);
}

void Puresu::draw(DrawContext &ctx)
{
    // TODO: paint three pos
    if (!isVisible || length <= 0)
    {
        return;
    }
    PolygonShape head, tail, body;
    vec3 right, upLen, rightLen, ctr, centerMove, bCenter, tCenter;

    glm_vec3_cross(targetSlot->up, targetSlot->normal, right);
    glm_vec3_normalize(right);
    glm_vec3_scale(right, FLAT_NOTE_SIZE, rightLen);
    glm_vec3_scale(targetSlot->up, FLAT_NOTE_SIZE / 2, upLen);
    glm_vec3_sub(basePosition, upLen, ctr);
    mkRectPoints(head, ctr, upLen, rightLen);
    head.renderPreset = RECT;
    head.shader = "rect";
    head.texture = "puresu-head";

    glm_vec3_scale(targetSlot->up, length * BASE_FALL_SPEED, upLen);
    glm_vec3_scale(upLen, 0.5, centerMove);
    glm_vec3_add(basePosition, centerMove, bCenter);
    mkRectPoints(body, bCenter, centerMove, rightLen);
    body.renderPreset = RECT;
    body.shader = "rect";
    body.texture = "puresu-body";

    glm_vec3_add(bCenter, centerMove, tCenter);
    glm_vec3_scale(targetSlot->up, FLAT_NOTE_SIZE / 2, upLen);
    glm_vec3_add(tCenter, upLen, tCenter);
    mkRectPoints(tail, tCenter, upLen, rightLen);
    tail.renderPreset = RECT;
    tail.shader = "rect";
    tail.texture = "puresu-tail";

    ctx.polygons.push_back(body);
    ctx.polygons.push_back(tail);
    ctx.polygons.push_back(head);
}

void Hoshi::draw(DrawContext &ctx)
{
    if (!isVisible)
    {
        return;
    }
    vec3 rightVec, upVec, normVec;
    vec3 points[6]; // NS, LR, FB
    glm_cross(targetSlot->up, targetSlot->normal, rightVec);
    glm_vec3_normalize(rightVec);
    glm_vec3_scale(rightVec, HOSHI_NOTE_SIZE, rightVec);
    glm_vec3_scale(targetSlot->up, HOSHI_NOTE_SIZE, upVec);
    glm_vec3_scale(targetSlot->normal, HOSHI_NOTE_SIZE, normVec);
    glm_vec3_add(basePosition, rightVec, points[3]);
    glm_vec3_add(basePosition, upVec, points[5]);
    glm_vec3_sub(basePosition, upVec, points[4]);
    glm_vec3_sub(basePosition, rightVec, points[2]);
    glm_vec3_add(basePosition, normVec, points[0]);
    glm_vec3_sub(basePosition, normVec, points[1]);
    PolygonShape ps;
    ps.isOpaque = false;
    ps.renderPreset = OCT;
    ps.shader = "hoshi";
    ps.texture = "hoshi";
    for (int i = 0; i < 6; i++)
    {
        ps.points.push_back(std::to_array(points[i]));
    }
    ctx.polygons.push_back(ps);

    // Draw the assist ring
    // TODO: obtain space assist time from chart
    if (assistRingScale > 0 && assistRingScale <= 1)
    {
        PolygonShape assist;
        assist.renderPreset = RECT;
        assist.shader = "assist-ring";
        assist.texture = "space-assist";
        assist.values["opacity"] = 1 - assistRingScale;

        auto size = ASSIST_RING_SIZE * assistRingScale;
        vec3 rdh;
        glm_vec3_normalize(rightVec);
        glm_vec3_scale(rightVec, size, rightVec);
        glm_vec3_scale(targetSlot->up, size, rdh);
        mkRectPoints(assist, targetSlot->center, rdh, rightVec);
        ctx.polygons.push_back(assist);
    }
}

void Hashi::draw(DrawContext &ctx)
{
    if (!isVisible)
    {
        return;
    }
    if (length <= 0)
    {
        return;
    }
    vec3 right;
    vec3 btmPoints[6], headPoints[6]; // CCW start from right
    glm_vec3_cross(targetSlot->up, targetSlot->normal, right);
    glm_vec3_normalize(right);
    glm_vec3_scale(right, HASHI_NOTE_SIZE, right);

    PolygonShape ps;
    for (int i = 0; i < 6; i++)
    {
        glm_vec3_add(basePosition, right, btmPoints[i]);
        ps.points.push_back(std::to_array(btmPoints[i]));
        if (i != 5)
        {
            glm_vec3_rotate(right, glm_rad(60.0), targetSlot->normal);
        }
    }

    auto xlen = length * BASE_FALL_SPEED;
    vec3 upLength;
    glm_vec3_copy(targetSlot->normal, upLength);
    glm_vec3_normalize(upLength);

    // Long Hashi notes require cutting, or render will not be correct
    if (xlen > 2 * HASHI_CUT_THRESHOLD)
    {
        int pieces = (int)(xlen / HASHI_CUT_THRESHOLD);
        float pcf = xlen / pieces;
        vec3 bufferPointsDown[6], bufferPointsUp[6];
        PolygonShape pse[pieces];
        // Make the bottom one
        glm_vec3_scale(upLength, pcf, upLength);
        for (int i = 0; i < 6; i++)
        {
            pse[0].points.push_back(std::to_array(btmPoints[i]));
        }
        for (int i = 0; i < 6; i++)
        {
            glm_vec3_add(btmPoints[i], upLength, bufferPointsDown[i]);
            pse[0].points.push_back(std::to_array(bufferPointsDown[i]));
        }
        pse[0].renderPreset = PRISM_BTM;

        // Make middle ones
        for (int i = 1; i < pieces - 1; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                pse[i].points.push_back(std::to_array(bufferPointsDown[j]));
            }
            for (int j = 0; j < 6; j++)
            {
                glm_vec3_add(bufferPointsDown[j], upLength, bufferPointsUp[j]);
                pse[i].points.push_back(std::to_array(bufferPointsUp[j]));
                glm_vec3_copy(bufferPointsUp[j], bufferPointsDown[j]);
            }
            pse[i].renderPreset = PRISM_SIDE;
        }

        // Make the top one
        for (int j = 0; j < 6; j++)
        {
            pse[pieces - 1].points.push_back(std::to_array(bufferPointsDown[j]));
        }
        // Correct mistakes
        glm_vec3_normalize(upLength);
        glm_vec3_scale(upLength, xlen, upLength);
        for (int i = 0; i < 6; i++)
        {
            glm_vec3_add(btmPoints[i], upLength, headPoints[i]);
            pse[pieces - 1].points.push_back(std::to_array(headPoints[i]));
        }
        pse[pieces - 1].renderPreset = PRISM_HAT;

        // Push all
        for (int i = 0; i < pieces; i++)
        {
            pse[i].isOpaque = false;
            pse[i].shader = "hashi";
            pse[i].texture = "hashi-hat";
            pse[i].subTexture = "hashi-side";
            ctx.polygons.push_back(pse[i]);
        }
    }
    else
    {
        // Use PRISM-FULL to draw
        glm_vec3_scale(upLength, xlen, upLength);
        for (int i = 0; i < 6; i++)
        {
            glm_vec3_add(btmPoints[i], upLength, headPoints[i]);
            ps.points.push_back(std::to_array(headPoints[i]));
        }

        ps.renderPreset = PRISM_FULL;
        ps.isOpaque = false;
        ps.shader = "hashi";
        ps.texture = "hashi-hat";
        ps.subTexture = "hashi-side";
        ctx.polygons.push_back(ps);
    }
    if (assistRingScale > 0 && assistRingScale <= 1)
    {
        PolygonShape assist;
        assist.renderPreset = RECT;
        assist.shader = "assist-ring";
        assist.texture = "space-assist";
        assist.values["opacity"] = 1 - assistRingScale;

        auto size = ASSIST_RING_SIZE * assistRingScale;
        vec3 rdh, rightVec;
        glm_vec3_cross(targetSlot->up, targetSlot->normal, rightVec);
        glm_vec3_normalize(rightVec);
        glm_vec3_scale(rightVec, size, rightVec);
        glm_vec3_scale(targetSlot->up, size, rdh);
        mkRectPoints(assist, targetSlot->center, rdh, rightVec);
        ctx.polygons.push_back(assist);
    }
}

void Tapu::performJudge(double absTime, InputSet &input, ScoreManager &sm)
{
    if (isFake)
    {
        return;
    }
    switch (jStage)
    {
    case BUSY:
        if (input.keyInfo[keyCode] == 0)
        {
            jStage = CLEAR;
        }
        if (!isOverlapped(hitTime, sm.rules.judgeTime.range, absTime, 0) && absTime > hitTime)
        {
            // Lost
            sm.addJudgeGrade(LT, TAPU);
            jStage = JUDGED;
        }
        break;
    case CLEAR:
        // Accepting judge
        if (isOverlapped(hitTime, sm.rules.judgeTime.range, absTime, 0))
        {
            jStage = ACTIVE;
        }
        else
        {
            if (absTime > hitTime)
            {
                // Lost
                sm.addJudgeGrade(LT, TAPU);
                jStage = JUDGED;
            }
            else if (input.keyInfo[keyCode] == 1)
            {
                // Too early
                jStage = BUSY;
            }
        }
        break;
    case ACTIVE:
        if (input.keyInfo[keyCode] == 1)
        {
            targetSlot->inUse = true;
            // Let's do this
            jStage = JUDGED;
            if (isOverlapped(hitTime, sm.rules.judgeTime.perfect, absTime, 0))
            {
                sm.addJudgeGrade(PF, TAPU);
            }
            else if (isOverlapped(hitTime, sm.rules.judgeTime.almost, absTime, 0))
            {
                sm.addJudgeGrade(AT, TAPU);
            }
            else if (isOverlapped(hitTime, sm.rules.judgeTime.good, absTime, 0))
            {
                sm.addJudgeGrade(AC, TAPU);
            }
            else
            {
                sm.addJudgeGrade(TC, TAPU);
            }
        }
        else if (!isOverlapped(hitTime, sm.rules.judgeTime.range, absTime, 0))
        {
            jStage = CLEAR; // Ready to lost
        }
        break;
    case JUDGED:
    default:
        // TODO: play animantion and post-process
        isVisible = false;
        return;
    }
}

void Shizuku::performJudge(double absTime, InputSet &input, ScoreManager &sm)
{
    if (isFake)
    {
        return;
    }
    switch (jStage)
    {
    case JUDGED:
        // TODO: add animation
        isVisible = false;
        return;
    default:
        // In all other cases, check and judge
        if (isOverlapped(hitTime, SHIZUKU_JUDGE_WINDOW, absTime, 0))
        {

            if (input.keyInfo[keyCode] == 1)
            {
                // You got it
                targetSlot->inUse = true;
                sm.addJudgeGrade(PF, SZKU);
                jStage = JUDGED;
            }
        }
        else
        {
            if (absTime > hitTime)
            {
                // You failed!
                sm.addJudgeGrade(LT, SZKU);
                jStage = JUDGED;
            }
        }
    }
}

void Puresu::performJudge(double absTime, InputSet &input, ScoreManager &sm)
{
    if (isFake)
    {
        return;
    }
    switch (jStage)
    {
    case JUDGED:
    default:
        isVisible = false;
        return;
    case BUSY:
        if (input.keyInfo[keyCode] == 0)
        {
            jStage = CLEAR;
        }
        if (!isOverlapped(hitTime, sm.rules.judgeTime.range, absTime, 0) && absTime > hitTime)
        {
            // Lost
            sm.addJudgeGrade(LT, TAPU);
            jStage = JUDGED;
        }
        break;
    case CLEAR:
        // Accepting judge
        if (isOverlapped(hitTime, sm.rules.judgeTime.range, absTime, 0) && input.keyInfo[keyCode] == 1)
        {
            // Turn to active, keep pressing!
            jStage = ACTIVE;
        }
        else
        {
            if (absTime > hitTime && !isOverlapped(hitTime, sm.rules.judgeTime.range, absTime, 0))
            {
                // Lost
                sm.addJudgeGrade(LT, TAPU);
                jStage = JUDGED;
            }
            else if (input.keyInfo[keyCode] == 1)
            {
                // Too early
                jStage = BUSY;
            }
        }
        break;
    case ACTIVE:
        if (absTime >= hitTime + absLength)
        {
            // Well done!
            sm.addJudgeGrade(PF, PRSU);
            jStage = JUDGED;
            return;
        }
        if (input.keyInfo[keyCode] == 0)
        {
            if (isOverlapped((hitTime + absLength), sm.rules.judgeTime.almost, absTime, 0))
            {
                // AT
                sm.addJudgeGrade(AT, PRSU);
                jStage = JUDGED;
                return;
            }
            else if (isOverlapped((hitTime + absLength), sm.rules.judgeTime.good, absTime, 0))
            {
                sm.addJudgeGrade(AC, PRSU);
                jStage = JUDGED;
                return;
            }
            // Do not refresh time
        }
        else
        {
            targetSlot->inUse = true; // Keep the animation playing
            lastSuccJudge = absTime;  // Refresh time
        }
        if (lastSuccJudge == -1)
        {
            lastSuccJudge = absTime; // Reset flag
        }
        if (lastSuccJudge != -1 && absTime - lastSuccJudge > sm.rules.judgeTime.allowBreak)
        {
            if (lastSuccJudge - hitTime > 0.5 * absTime)
            {
                sm.addJudgeGrade(MD, PRSU);
            }
            else
            {
                // At least you've got to ACTIVE
                sm.addJudgeGrade(TC, PRSU);
            }
            jStage = JUDGED;
        }

        break;
    }
}

void Puresu::tick(double absTime)
{
    if (!isVisible)
    {
        length = 0;
        return;
    }
    AbstractNote::tick(absTime);
    if (absTime >= hitTime)
    {
        if (absTime <= hitTime + absLength)
        {
            glm_vec3_copy(targetSlot->center, basePosition);
            length = absLength - (absTime - hitTime);
        }
        else
        {
            length = 0;
        }
    }
    else
    {
        length = absLength;
    }
}

void Hoshi::performJudge(double absTime, InputSet &input, ScoreManager &sm)
{
    // Almost the same as Tapu
    if (isFake)
    {
        return;
    }
    switch (jStage)
    {
    case BUSY:
        if (input.keyInfo[keyCode] == 0)
        {
            jStage = CLEAR;
        }
        if (!isOverlapped(hitTime, sm.rules.judgeTime.range, absTime, 0) && absTime > hitTime)
        {
            sm.addJudgeGrade(LT, HOSHI);
            jStage = JUDGED;
        }
        break;
    case CLEAR:
        if (isOverlapped(hitTime, sm.rules.judgeTime.range, absTime, 0))
        {
            jStage = ACTIVE;
        }
        else
        {
            if (absTime > hitTime)
            {
                sm.addJudgeGrade(LT, HOSHI);
                jStage = JUDGED;
            }
            else if (input.keyInfo[keyCode] == 1)
            {
                // Too early
                jStage = BUSY;
            }
        }
        break;
    case ACTIVE:
        if (input.keyInfo[keyCode] == 1)
        {
            targetSlot->inUse = true;
            jStage = JUDGED;
            if (isOverlapped(hitTime, sm.rules.judgeTime.perfect, absTime, 0))
            {
                sm.addJudgeGrade(PF, HOSHI);
            }
            else if (isOverlapped(hitTime, sm.rules.judgeTime.almost, absTime, 0))
            {
                sm.addJudgeGrade(AT, HOSHI);
            }
            else if (isOverlapped(hitTime, sm.rules.judgeTime.good, absTime, 0))
            {
                sm.addJudgeGrade(AC, HOSHI);
            }
            else
            {
                sm.addJudgeGrade(TC, HOSHI);
            }
        }
        else if (!isOverlapped(hitTime, sm.rules.judgeTime.range, absTime, 0))
        {
            jStage = CLEAR;
        }
        break;
    case JUDGED:
    default:
        // TODO: play animantion and post-process
        isVisible = false;
        return;
    }
}

void Hoshi::tick(double absTime)
{
    if (!isVisible)
    {
        return;
    }
    auto det = hitTime - absTime;
    if (det > 0 && det < ASSIST_RING_TIME)
    {
        assistRingScale = det / ASSIST_RING_TIME + 0.1; // Minor adjustment
    }
    else
    {
        assistRingScale = -1;
    }
    auto len = det * BASE_FALL_SPEED;
    vec3 shift;
    glm_vec3_scale(targetSlot->normal, len, shift);
    glm_vec3_add(targetSlot->center, shift, basePosition);
}

void Hashi::performJudge(double absTime, InputSet &input, ScoreManager &sm)
{
    if (isFake)
    {
        return;
    }
    switch (jStage)
    {
    case JUDGED:
        // TODO: add animation
        isVisible = false;
        return;
    default:
        if (absTime > hitTime && absTime < hitTime + absLength)
        {
            if (lastSuccJudge == -1)
            {
                lastSuccJudge = absTime;
                return;
            }
            if (input.keyInfo[keyCode] == 1)
            {
                targetSlot->inUse = true;
                judgedLength += (absTime - lastSuccJudge);
            }
            lastSuccJudge = absTime;
        }
        else if (absTime > hitTime + absLength)
        {
            jStage = JUDGED;
            auto comRate = judgedLength / absLength;
            if (comRate > 0.95)
            {
                sm.addJudgeGrade(PF, HASHI);
            }
            else if (comRate > 0.85)
            {
                sm.addJudgeGrade(AT, HASHI);
            }
            else if (comRate > 0.70)
            {
                sm.addJudgeGrade(AC, HASHI);
            }
            else if (comRate > 0.45)
            {
                sm.addJudgeGrade(MD, HASHI);
            }
            else if (comRate > 0)
            {
                sm.addJudgeGrade(TC, HASHI);
            }
            else
            {
                sm.addJudgeGrade(LT, HASHI);
            }
        }
    }
}

void Hashi::tick(double absTime)
{
    if (!isVisible)
    {
        return;
    }
    auto det = hitTime - absTime;
    if (det > 0 && det < ASSIST_RING_TIME)
    {
        assistRingScale = det / ASSIST_RING_TIME + 0.2; // Minor adjustment
    }
    else
    {
        assistRingScale = -1;
    }
    auto len = det * BASE_FALL_SPEED;
    vec3 shift;
    glm_vec3_scale(targetSlot->normal, len, shift);
    glm_vec3_add(targetSlot->center, shift, basePosition);
    if (absTime >= hitTime)
    {
        if (absTime <= hitTime + absLength)
        {
            glm_vec3_copy(targetSlot->center, basePosition);
            length = absLength - (absTime - hitTime);
        }
        else
        {
            length = 0;
        }
    }
    else
    {
        length = absLength;
    }
}