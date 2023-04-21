#include "Slot.hh"
#include "Note.hh"

#include <cstdlib>

#define SLOT_SIZE 1.2
#define SLOT_SINK_THRESHOLD 0.001
#define HIT_EFFECT_SIZE 1.8
#define HIT_EFFECT_LIFETIME 0.4
#define HIT_EFFECT_GEN_INTERVAL 0.2

void Slot::draw(DrawContext &ctx)
{
    if (!isVisible)
    {
        return;
    }
    PolygonShape p;
    p.renderPreset = RECT;
    p.shader = "rect";
    p.texture = "slot";
    vec3 lt, lb, rt, rb, right, ctr;
    vec3 dw, dh, t;
    glm_vec3_cross(up, normal, right);
    glm_normalize(right);
    glm_vec3_scale(right, SLOT_SIZE, dw);
    glm_vec3_scale(up, SLOT_SIZE / 2, dh);
    glm_vec3_sub(center, dh, ctr);

    glm_vec3_add(ctr, dw, t);
    glm_vec3_add(t, dh, rt);

    glm_vec3_sub(ctr, dw, t);
    glm_vec3_add(t, dh, lt);

    glm_vec3_sub(ctr, dw, t);
    glm_vec3_sub(t, dh, lb);

    glm_vec3_sub(ctr, dh, t);
    glm_vec3_add(t, dw, rb);

    p.points.push_back(std::to_array(lt));
    p.points.push_back(std::to_array(lb));
    p.points.push_back(std::to_array(rt));
    p.points.push_back(std::to_array(rb));
    ctx.polygons.push_back(p);

    for (auto &h : hitEffects)
    {
        h->draw(ctx);
    }
}

void Slot::tick(double absTime)
{
    // Generating
    if (inUse)
    {
        inUse = false; // Will be set to true in the next tick
        if (absTime - lastGenTime >= HIT_EFFECT_GEN_INTERVAL)
        {
            HitEffect *he = new HitEffect;
            he->targetSlot = this;
            he->isVisible = true;
            he->startTime = absTime;
            hitEffects.insert(he);
            lastGenTime = absTime;
        }
    }
    else
    {
        lastGenTime = 0;
    }

    // Ticking
    for (auto &h : hitEffects)
    {
        h->tick(absTime);
        if (!h->isVisible)
        {
            hitEffects.erase(h);
            delete h;
        }
    }
}

HitEffect::HitEffect()
{
    initDirection = rand() % 4;
}

void HitEffect::tick(double absTime)
{
    if (!isVisible)
    {
        return;
    }
    double dur = absTime - startTime;
    if (dur > HIT_EFFECT_LIFETIME)
    {
        isVisible = false;
        return;
    }
    auto pct = std::pow(dur / HIT_EFFECT_LIFETIME, 0.6) * 0.8 + 0.2;
    size = pct * HIT_EFFECT_SIZE;
    // opacity = (1 - pct) * 0.4 + 0.6;
}

void HitEffect::draw(DrawContext &ctx)
{
    PolygonShape pg;
    pg.renderPreset = RECT;
    pg.isOpaque = true;
    pg.shader = "hit-effect";
    pg.texture = "hit-effect";
    // pg.values["alpha"] = opacity;
    vec3 tUp, tRight;
    glm_vec3_copy(targetSlot->up, tUp);
    glm_vec3_normalize(tUp);
    glm_vec3_rotate(tUp, glm_rad(initDirection * 90), targetSlot->normal);
    glm_vec3_cross(tUp, targetSlot->normal, tRight);
    glm_vec3_normalize(tRight);
    glm_vec3_scale(tUp, size, tUp);
    glm_vec3_scale(tRight, size, tRight);
    mkRectPoints(pg, targetSlot->center, tUp, tRight);
    ctx.polygons.push_back(pg);
}