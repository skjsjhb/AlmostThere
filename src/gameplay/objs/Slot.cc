#include "Slot.hh"
#include "Note.hh"

#include <cstdlib>

#define SLOT_SIZE 1.2
#define SLOT_SINK_THRESHOLD 0.001

static std::vector<std::string> slotTexName = {"circle", "eureka", "line"};

void Slot::draw(DrawContext &ctx)
{
    if (!isVisible)
    {
        return;
    }
    Polygon p;
    p.renderPreset = RECT;
    p.shader = "rect";
    p.texture = slotTexName[variant];
    vec3 lt, lb, rt, rb, right, ctr;
    vec3 dw, dh, t;
    glm_vec3_cross(up, normal, right);
    glm_normalize(right);
    glm_vec3_scale(right, SLOT_SIZE, dw);
    glm_vec3_scale(up, variant == CIRCLE ? SLOT_SIZE : SLOT_SIZE / 2, dh);
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
}

void Slot::tick(double absTime)
{
    controller->tick(absTime);
    glm_vec3_copy(controller->currentStatus.pos, center);
    glm_vec3_copy(controller->currentStatus.up, up);
    glm_vec3_copy(controller->currentStatus.normal, normal);
}
