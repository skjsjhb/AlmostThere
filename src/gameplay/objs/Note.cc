#include "Note.hh"
#include "util/Util.hh"

#include <array>

#define BASE_FALL_SPEED 10.0
#define NOTE_SIZE 1.0

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

static void mkRectPoints(PolygonShape &pg, vec3 center, vec3 upLen, vec3 rightLen)
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
    vec3 pCenter;
    glm_vec3_cross(obj->targetSlot->up, obj->targetSlot->normal, right);
    glm_vec3_normalize(right);
    glm_vec3_scale(right, NOTE_SIZE, dw);
    glm_vec3_scale(obj->targetSlot->up, NOTE_SIZE, dh);

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
    glm_vec3_scale(right, NOTE_SIZE, rightLen);
    glm_vec3_scale(targetSlot->up, NOTE_SIZE / 2, upLen);
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
    glm_vec3_scale(targetSlot->up, NOTE_SIZE / 2, upLen);
    glm_vec3_add(tCenter, upLen, tCenter);
    mkRectPoints(tail, tCenter, upLen, rightLen);
    tail.renderPreset = RECT;
    tail.shader = "rect";
    tail.texture = "puresu-tail";

    ctx.polygons.push_back(body);
    ctx.polygons.push_back(tail);
    ctx.polygons.push_back(head);
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
            lastSuccJudge = absTime; // Refresh time
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