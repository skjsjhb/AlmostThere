#include "Slot.hh"

#define NOTE_SIZE 0.2
#define PANEL_SIZE 10
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
    vec3 lt, lb, rt, rb, right;
    vec3 dw, dh, t;
    glm_vec3_cross(up, normal, right);
    glm_normalize(right);
    glm_vec3_scale(right, NOTE_SIZE, dw);
    glm_vec3_scale(up, NOTE_SIZE, dh);
    glm_vec3_add(center, dw, t);
    glm_vec3_add(t, dh, rt);

    glm_vec3_sub(center, dw, t);
    glm_vec3_add(t, dh, lt);

    glm_vec3_sub(center, dw, t);
    glm_vec3_sub(t, dh, lb);

    glm_vec3_sub(center, dh, t);
    glm_vec3_add(t, dw, rb);

    p.points.push_back(std::to_array(lt));
    p.points.push_back(std::to_array(lb));
    p.points.push_back(std::to_array(rt));
    p.points.push_back(std::to_array(rb));
    ctx.polygons.push_back(p);
}

void Slot::tick(double absTime)
{
}

void Panel::draw(DrawContext &ctx)
{
    PolygonShape p;
    p.renderPreset = RECT;
    p.shader = "panel";
    p.texture = "panel";
    vec3 right,
        dw, dh, lt, lb, rt, rb, t;
    glm_vec3_cross(up, normal, right);
    glm_vec3_normalize(right);
    glm_vec3_scale(right, PANEL_SIZE, dw);
    glm_vec3_scale(up, PANEL_SIZE, dh);

    glm_vec3_add(basePoint, dw, t);
    glm_vec3_add(t, dh, rt);

    glm_vec3_sub(basePoint, dw, t);
    glm_vec3_add(t, dh, lt);

    glm_vec3_sub(basePoint, dw, t);
    glm_vec3_sub(t, dh, lb);

    glm_vec3_sub(basePoint, dh, t);
    glm_vec3_add(t, dw, rb);

    p.points.push_back(std::to_array(lt));
    p.points.push_back(std::to_array(lb));
    p.points.push_back(std::to_array(rt));
    p.points.push_back(std::to_array(rb));
    ctx.polygons.push_back(p);
}

void Panel::tick(double absTime)
{
}