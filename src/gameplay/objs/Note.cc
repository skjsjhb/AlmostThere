#include "Note.hh"
#include "util/Util.hh"
#include "gameplay/view/View.hh"
#include "gameplay/base/Game.hh"
#include <array>
#include "spdlog/spdlog.h"

using namespace spdlog;

#define BASE_FALL_SPEED 10.0
#define FLAT_NOTE_SIZE 1.0
#define HOSHI_NOTE_SIZE 0.8
#define HASHI_NOTE_SIZE 0.6
#define ASSIST_RING_SIZE 0.6
#define HASHI_CUT_THRESHOLD 1.0
#define ASSIST_RING_TIME 1.0
#define HIT_EFFECT_SIZE 1.8
#define HIT_EFFECT_LIFETIME 0.4
#define HIT_EFFECT_GEN_INTERVAL 0.2
#define NOTE_FLOAT_THRESHOLD 0.01
#define SHIZUKU_JUDGE_WINDOW 0.01

void Note::tick(double absTime)
{
    TickObject::tick(absTime);
    performJudge(absTime, game.input, game.score);
}

std::shared_ptr<Note> Note::createNote(std::weak_ptr<NoteObject> obj, Game &g)
{
    std::shared_ptr<Note> a;
    auto xobj = obj.lock();
    switch (xobj->noteType)
    {
    case TAPU:
        a = std::make_shared<Tapu>(g);
        break;
    case SZKU:
        a = std::make_shared<Shizuku>(g);
        break;
    case HOSHI:
        a = std::make_shared<Hoshi>(g);
        break;
    case PRSU:
        a = std::make_shared<Puresu>(g);
        break;
    case HASHI:
        a = std::make_shared<Hashi>(g);
        break;
    case KZTU:
        a = std::make_shared<Kyozetsu>(g);
        break;
    case AKU:
    case SGKI:
    case TSAR:
    default:
        warn("Creating note with missing type: " + std::to_string(xobj->noteType));
        break;
    }
    a->controller = std::make_shared<ObjController>(*xobj);
    return a;
}

// Insert corners of a rectangle into specified polygon
static void mkRectPoints(Polygon &pg, vec3 center, vec3 upLen, vec3 rightLen)
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

// Paint a rectangle with texture
static void drawRect(DrawContext &ctx, const std::string sdName, vec3 pos, vec3 up, vec3 norm)
{
    vec3 right;
    vec3 dw, dh;
    glm_vec3_cross(up, norm, right);
    glm_vec3_normalize(right);
    glm_vec3_scale(right, FLAT_NOTE_SIZE, dw);
    glm_vec3_scale(up, FLAT_NOTE_SIZE, dh);

    Polygon pg;
    mkRectPoints(pg, pos, dh, dw);
    pg.renderPreset = RECT;
    pg.shader = "rect";
    pg.texture = sdName;
    ctx.polygons.push_back(std::move(pg));
}

// Check if this position is 'pressed' through mouse ray casting
// TODO: deprecated ScoreManager
static bool verifyPressed(vec3 pos, Game &g)
{
    if (g.view.camera.expired())
    {
        return false;
    }
    vec3 cPos, dir;
    g.view.camera.lock()->getPosition(cPos);
    glm_vec3_sub(pos, cPos, dir);
    glm_vec3_normalize(dir);
    for (auto &i : g.input.touchPoints)
    {
        vec3 ray;
        vec2 pos = {i[0], i[1]};
        castMouseRay(g.view, pos, ray);
        glm_vec3_normalize(ray);
        double ang = glm_vec3_dot(ray, dir);
        if (ang >= (1 - g.score.rules.inputOptn.judgeRayAngle))
        {
            return true;
        }
    }
    return false;
}

void Tapu::draw(DrawContext &ctx)
{
    if (!isActive)
    {
        return;
    }
    auto stat = controller->getState();
    drawRect(ctx, "tapu", stat.pos, stat.up, stat.normal);
}

void Shizuku::draw(DrawContext &ctx)
{
    if (!isActive)
    {
        return;
    }
    auto stat = controller->getState();
    drawRect(ctx, "shizuku", stat.pos, stat.up, stat.normal);
}

void Puresu::draw(DrawContext &ctx)
{
    auto stat = controller->getState();
    if (!isActive || stat.len <= 0)
    {
        return;
    }
    Polygon head, tail, body;
    vec3 right, upLen, rightLen, ctr, centerMove, bCenter, tCenter;

    glm_vec3_cross(stat.up, stat.normal, right);
    glm_vec3_normalize(right);
    glm_vec3_scale(right, FLAT_NOTE_SIZE, rightLen);
    glm_vec3_scale(stat.up, FLAT_NOTE_SIZE / 2, upLen);
    glm_vec3_sub(stat.pos, upLen, ctr);
    mkRectPoints(head, ctr, upLen, rightLen);
    head.renderPreset = RECT;
    head.shader = "rect";
    head.texture = "puresu-head";

    glm_vec3_scale(stat.up, stat.len * BASE_FALL_SPEED, upLen);
    glm_vec3_scale(upLen, 0.5, centerMove);
    glm_vec3_add(stat.pos, centerMove, bCenter);
    mkRectPoints(body, bCenter, centerMove, rightLen);
    body.renderPreset = RECT;
    body.shader = "rect";
    body.texture = "puresu-body";

    glm_vec3_add(bCenter, centerMove, tCenter);
    glm_vec3_scale(stat.up, FLAT_NOTE_SIZE / 2, upLen);
    glm_vec3_add(tCenter, upLen, tCenter);
    mkRectPoints(tail, tCenter, upLen, rightLen);
    tail.renderPreset = RECT;
    tail.shader = "rect";
    tail.texture = "puresu-tail";

    ctx.polygons.push_back(std::move(body));
    ctx.polygons.push_back(std::move(tail));
    ctx.polygons.push_back(std::move(head));
}

void Kyozetsu::draw(DrawContext &ctx)
{
    if (!isActive)
    {
        return;
    }
    auto stat = controller->getState();
    drawRect(ctx, "kyozetsu", stat.pos, stat.up, stat.normal);
}

void Hoshi::draw(DrawContext &ctx)
{
    if (!isActive)
    {
        return;
    }
    vec3 rightVec, upVec, normVec;
    vec3 points[6]; // NS, LR, FB
    auto stat = controller->getState();
    glm_cross(stat.up, stat.normal, rightVec);
    glm_vec3_normalize(rightVec);
    glm_vec3_scale(rightVec, HOSHI_NOTE_SIZE, rightVec);
    glm_vec3_scale(stat.up, HOSHI_NOTE_SIZE, upVec);
    glm_vec3_scale(stat.normal, HOSHI_NOTE_SIZE, normVec);
    glm_vec3_add(stat.pos, rightVec, points[3]);
    glm_vec3_add(stat.pos, upVec, points[5]);
    glm_vec3_sub(stat.pos, upVec, points[4]);
    glm_vec3_sub(stat.pos, rightVec, points[2]);
    glm_vec3_add(stat.pos, normVec, points[0]);
    glm_vec3_sub(stat.pos, normVec, points[1]);

    Polygon ps;
    ps.renderPreset = OCT;
    ps.shader = "hoshi";
    ps.texture = "hoshi";
    for (int i = 0; i < 6; i++)
    {
        ps.points.push_back(std::to_array(points[i]));
    }
    ctx.polygons.push_back(std::move(ps));
}

void Hashi::draw(DrawContext &ctx)
{
    auto stat = controller->getState();
    if (!isActive || stat.len <= 0)
    {
        return;
    }
    vec3 right;
    vec3 btmPoints[6], headPoints[6]; // CCW start from right

    glm_vec3_cross(stat.up, stat.normal, right);
    glm_vec3_normalize(right);
    glm_vec3_scale(right, HASHI_NOTE_SIZE, right);

    Polygon ps;
    for (int i = 0; i < 6; i++)
    {
        glm_vec3_add(stat.pos, right, btmPoints[i]);
        ps.points.push_back(std::to_array(btmPoints[i]));
        if (i != 5)
        {
            glm_vec3_rotate(right, glm_rad(60.0), stat.normal);
        }
    }

    vec3 upLength;
    glm_vec3_copy(stat.normal, upLength);
    glm_vec3_normalize(upLength);

    // In the past, long Hashi notes requires cutting in order to render them correctly.
    // Now Hashi are opaque and there is no need to calculate this anymore.
    glm_vec3_scale(upLength, stat.len * BASE_FALL_SPEED, upLength);
    for (int i = 0; i < 6; i++)
    {
        // Get corresponding point on the head face
        glm_vec3_add(btmPoints[i], upLength, headPoints[i]);
        ps.points.push_back(std::to_array(headPoints[i]));
    }

    ps.renderPreset = PRISM_FULL;
    ps.shader = "hashi";
    ps.texture = "hashi-hat";
    ps.subTexture = "hashi-side";
    ctx.polygons.push_back(std::move(ps));
}

void Shizuku::performJudge(double absTime, InputSet &input, ScoreManager &sm)
{
    if (isFake)
    {
        return;
    }

    auto stat = controller->getState();
    auto &ref = controller->getReference();

    switch (judgeStage)
    {
    case JUDGED:
        isActive = false;
        return;
    default:
        // In all other cases, check and judge
        if (isOverlapped(ref.endTime, SHIZUKU_JUDGE_WINDOW, absTime, 0))
        {

            if (verifyPressed(stat.pos, game))
            {
                // You got it
                sm.addJudgeGrade(PF, SZKU);
                judgeStage = JUDGED;
            }
        }
        else
        {
            if (absTime > ref.endTime)
            {
                // You failed!
                sm.addJudgeGrade(LT, SZKU);
                judgeStage = JUDGED;
            }
        }
    }
}

void Kyozetsu::performJudge(double absTime, InputSet &input, ScoreManager &sm)
{
    if (isFake)
    {
        return;
    }

    auto stat = controller->getState();
    auto &ref = controller->getReference();

    switch (judgeStage)
    {
    case JUDGED:
        isActive = false;
        return;
    default:
        // In all other cases, check and judge
        if (isOverlapped(ref.endTime, sm.rules.judgeTime.good, absTime, 0))
        {

            if (verifyPressed(stat.pos, game))
            {
                // You touched the zone!
                sm.addJudgeGrade(LT, KZTU);
                judgeStage = JUDGED;
            }
        }
        else
        {
            if (absTime > ref.endTime + sm.rules.judgeTime.good)
            {
                // OK very well!
                sm.addJudgeGrade(PF, SZKU);
                judgeStage = JUDGED;
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

    auto range = sm.rules.judgeTime.range;
    auto almost = sm.rules.judgeTime.almost;
    auto good = sm.rules.judgeTime.good;

    auto stat = controller->getState();
    auto &ref = controller->getReference();

    switch (judgeStage)
    {
    case JUDGED:
    default:
        isActive = false;
        return;
    case BUSY:
        if (!verifyPressed(stat.pos, game))
        {
            judgeStage = CLEAR;
        }
        if (!isOverlapped(ref.endTime, range, absTime, 0) && absTime > ref.endTime)
        {
            // Lost
            sm.addJudgeGrade(LT, TAPU);
            judgeStage = JUDGED;
        }
        break;
    case CLEAR:
        // Accepting judge
        if (isOverlapped(ref.endTime, range, absTime, 0) && verifyPressed(stat.pos, game))
        {
            // Turn to active, keep pressing!
            judgeStage = ACTIVE;
        }
        else
        {
            if (absTime > ref.endTime && !isOverlapped(ref.endTime, range, absTime, 0))
            {
                // Lost
                sm.addJudgeGrade(LT, TAPU);
                judgeStage = JUDGED;
            }
            else if (verifyPressed(stat.pos, game))
            {
                // Too early
                judgeStage = BUSY;
            }
        }
        break;
    case ACTIVE:
        if (absTime >= ref.endTime + ref.length)
        {
            // Well done!
            sm.addJudgeGrade(PF, PRSU);
            judgeStage = JUDGED;
            return;
        }
        if (!verifyPressed(stat.pos, game))
        {
            if (isOverlapped((ref.endTime + ref.length), almost, absTime, 0))
            {
                // AT
                sm.addJudgeGrade(AT, PRSU);
                judgeStage = JUDGED;
                return;
            }
            else if (isOverlapped((ref.endTime + ref.length), good, absTime, 0))
            {
                sm.addJudgeGrade(AC, PRSU);
                judgeStage = JUDGED;
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
            if (lastSuccJudge - ref.endTime > 0.5 * ref.length)
            {
                sm.addJudgeGrade(MD, PRSU);
            }
            else
            {
                // At least you've got to ACTIVE
                sm.addJudgeGrade(TC, PRSU);
            }
            judgeStage = JUDGED;
        }

        break;
    }
}

void Hashi::performJudge(double absTime, InputSet &input, ScoreManager &sm)
{
    if (isFake)
    {
        return;
    }

    auto stat = controller->getState();
    auto &ref = controller->getReference();
    switch (judgeStage)
    {
    case JUDGED:
        isActive = false;
        return;
    default:
        if (absTime > ref.endTime && absTime < ref.endTime + ref.length)
        {
            if (lastSuccJudge == -1)
            {
                lastSuccJudge = absTime;
                return;
            }
            if (verifyPressed(stat.pos, game))
            {
                judgedLength += (absTime - lastSuccJudge);
            }
            lastSuccJudge = absTime;
        }
        else if (absTime > ref.endTime + ref.length)
        {
            judgeStage = JUDGED;
            auto comRate = judgedLength / ref.length;
            if (comRate > 0.95)
            {
                // TODO: PF specialization
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

void SingletonNote::performJudge(double absTime, InputSet &input, ScoreManager &sm)
{
    if (isFake)
    {
        // Fake notes are not judged
        return;
    }
    auto range = sm.rules.judgeTime.range;
    auto perfect = sm.rules.judgeTime.perfect;
    auto almost = sm.rules.judgeTime.almost;
    auto good = sm.rules.judgeTime.good;

    auto stat = controller->getState();
    auto &ref = controller->getReference();

    switch (judgeStage)
    {
    case BUSY:
        // We want the player to actually 'tap' the note, so it must be released first.

        // If it's already released, change its status.
        if (!verifyPressed(stat.pos, game))
        {
            judgeStage = CLEAR;
        }
        // If it's too late...
        if (absTime > ref.endTime + range)
        {
            sm.addJudgeGrade(LT, typ);
            judgeStage = JUDGED;
        }
        break;
    case CLEAR:
        // The note position is cleared, but the note is not in its judge window yet.
        // If it went into its window and can be judged, activate it.
        if (isOverlapped(ref.endTime, range, absTime, 0))
        {
            judgeStage = ACTIVE;
        }
        else
        {
            // If it's already too late...
            if (absTime > ref.endTime + range)
            {
                sm.addJudgeGrade(LT, typ);
                judgeStage = JUDGED;
            }
            // If the position is pressed again, but still too early, then reset it to BUSY.
            else if (verifyPressed(stat.pos, game))
            {
                // Too early
                judgeStage = BUSY;
            }
        }
        break;
    case ACTIVE:
        // Now! Now! Press it and earn score!
        if (verifyPressed(stat.pos, game))
        {
            judgeStage = JUDGED;

            if (isOverlapped(ref.endTime, perfect, absTime, 0))
            {
                // TODO: enable PF for Tactical characters only
                sm.addJudgeGrade(PF, typ);
            }
            else if (isOverlapped(ref.endTime, almost, absTime, 0))
            {
                // AT
                sm.addJudgeGrade(AT, typ);
            }
            else if (isOverlapped(ref.endTime, good, absTime, 0))
            {
                // AC
                sm.addJudgeGrade(AC, typ);
            }
            else
            {
                // Still in range, okay, at least you didn't miss it.
                sm.addJudgeGrade(TC, typ);
            }
        }
        else if (!isOverlapped(ref.endTime, range, absTime, 0))
        {
            // If this has turned to ACTIVE then the condition actually means it's already too late.
            // Ready to lost
            judgeStage = CLEAR;
        }
        break;
    case JUDGED:
    default:
        // End judge mission.
        isActive = false;
        return;
    }
}