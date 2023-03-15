#include "Note.hh"
#include "base/util/Util.hh"

#include <array>

#define BASE_FALL_SPEED 2
#define NOTE_SIZE 0.2

void Tapu::performJudge(double absTime, ScoreManager &sm)
{
}

void Tapu::tick(double absTime)
{
    if (absTime > hitTime)
    {
        isVisible = false;
        return;
    }
    // Obtain fall speed from chart
    auto scale = (hitTime - absTime) * BASE_FALL_SPEED;
    vec3 offset;
    // Calculate position
    glm_vec3_scale(targetSlot->up, scale, offset);
    glm_vec3_add(targetSlot->center, offset, basePosition);
}

void Tapu::draw(DrawContext &ctx)
{
    if (!isVisible)
    {
        return;
    }
    vec3 right;
    vec3 lt, rt, lb, rb;
    vec3 dw, dh;
    vec3 tmp;
    glm_vec3_cross(targetSlot->up, targetSlot->normal, right);
    glm_vec3_normalize(right);
    glm_vec3_scale(right, NOTE_SIZE, dw);
    glm_vec3_scale(targetSlot->up, NOTE_SIZE, dh);
    glm_vec3_add(basePosition, dw, tmp);
    glm_vec3_add(tmp, dh, rt);
    glm_vec3_sub(basePosition, dw, tmp);
    glm_vec3_add(tmp, dh, lt);
    glm_vec3_add(basePosition, dw, tmp);
    glm_vec3_sub(tmp, dh, rb);
    glm_vec3_sub(basePosition, dw, tmp);
    glm_vec3_sub(tmp, dh, lb);
    PolygonShape pg;
    pg.renderPreset = RECT;
    pg.shader = "rect";
    pg.texture = "tapu";
    pg.points.push_back(std::to_array(lt));
    pg.points.push_back(std::to_array(lb));
    pg.points.push_back(std::to_array(rt));
    pg.points.push_back(std::to_array(rb));

    ctx.polygons.push_back(pg);
}