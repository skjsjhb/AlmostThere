#include "Note.hh"
#include "util/Util.hh"

#include <array>

#define BASE_FALL_SPEED 10
#define NOTE_SIZE 1

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

static void commonRectDraw(DrawContext &ctx, const std::string sdName, AbstractNote *obj)
{
    if (!obj->isVisible)
    {
        return;
    }
    vec3 right;
    vec3 lt, rt, lb, rb;
    vec3 dw, dh, threshold;
    vec3 tmp, pCenter;
    glm_vec3_cross(obj->targetSlot->up, obj->targetSlot->normal, right);
    glm_vec3_normalize(right);
    glm_vec3_scale(obj->targetSlot->normal, NOTE_FLOAT_THRESHOLD, threshold); // Avoid overlap
    glm_vec3_add(obj->basePosition, threshold, pCenter);
    glm_vec3_scale(right, NOTE_SIZE, dw);
    glm_vec3_scale(obj->targetSlot->up, NOTE_SIZE, dh);
    glm_vec3_add(pCenter, dw, tmp);
    glm_vec3_add(tmp, dh, rt);
    glm_vec3_sub(pCenter, dw, tmp);
    glm_vec3_add(tmp, dh, lt);
    glm_vec3_add(pCenter, dw, tmp);
    glm_vec3_sub(tmp, dh, rb);
    glm_vec3_sub(pCenter, dw, tmp);
    glm_vec3_sub(tmp, dh, lb);
    PolygonShape pg;
    pg.renderPreset = RECT;
    pg.shader = "rect";
    pg.texture = sdName;
    pg.points.push_back(std::to_array(lt));
    pg.points.push_back(std::to_array(lb));
    pg.points.push_back(std::to_array(rt));
    pg.points.push_back(std::to_array(rb));
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