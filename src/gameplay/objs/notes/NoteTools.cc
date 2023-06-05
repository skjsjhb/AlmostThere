#include "NoteTools.hh"

#include "gameplay/view/View.hh"
#include "gameplay/rules/GameRules.hh"
#include "engine/virtual/Input.hh"
#include "util/Util.hh"
#include "spdlog/spdlog.h"

#define CAST_ANGLE_THRESHOLD 0.987

bool isPressed3D(const glm::vec3 &pos, View &v, const InputBuffer &ibuf) {
    if (v.camera.expired()) {
        spdlog::warn("View camera not found. Unable to do cast ray.");
        return false;
    }
    auto cPos = v.camera.lock()->getPosition();
    auto dir = glm::normalize(pos - cPos);

    for (auto &i: ibuf.touchPoints) {
        glm::vec2 posx = {i[0], i[1]};
        auto ray = castMouseRay(v, posx);
        auto ang = glm::dot(ray, dir);
        if (ang >= CAST_ANGLE_THRESHOLD) {
            return true;
        }
    }
    return false;
}

static bool isInTriangle(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c) {
    auto as_x = p[0] - a[0];
    auto as_y = p[1] - a[1];

    bool s_ab = (b[0] - a[0]) * as_y - (b[1] - a[1]) * as_x > 0;

    if (((c[0] - a[0]) * as_y - (c[1] - a[1]) * as_x > 0) == s_ab)
        return false;
    if (((c[0] - b[0]) * (p[1] - b[1]) - (c[1] - b[1]) * (p[0] - b[0]) > 0) != s_ab)
        return false;
    return true;
}

bool isPressed2D(const glm::vec3 origin[4], View &v, const InputBuffer &ibuf) {
    glm::vec2 ptsc[4];
    if (v.camera.expired()) {
        spdlog::warn("View camera not found. Unable to do 2D judge.");
        return false;
    }
    auto c = v.camera.lock();
    auto proj = c->getProjectionMatrix();
    auto view = c->getViewMatrix();
    for (int i = 0; i < 4; ++i) {
        glm::vec4 cr = {origin[i][0], origin[i][1], origin[i][2], 1};
        cr = proj * view * cr;
        ptsc[i][0] = cr[0] / cr[3];
        ptsc[i][1] = cr[1] / cr[3];
    }

    for (auto &i: ibuf.touchPoints) {
        glm::vec2 ndc;
        ndc[0] = (2.0f * i[0]) / v.screenSize[0] - 1.0f;
        ndc[1] = 1 - (2.0f * i[1]) / v.screenSize[1];
        if (isInTriangle(ndc, ptsc[0], ptsc[1], ptsc[2]))
            return true;
        if (isInTriangle(ndc, ptsc[0], ptsc[1], ptsc[3]))
            return true;
        if (isInTriangle(ndc, ptsc[0], ptsc[2], ptsc[3]))
            return true;
        if (isInTriangle(ndc, ptsc[1], ptsc[2], ptsc[3]))
            return true;
    }
    return false;
}

void createRect(const glm::vec3 &pos, const glm::vec3 &up, const glm::vec3 &norm, double sizew, double sizeh,
                glm::vec3 output[4]) {
    auto rightx = glm::normalize(glm::cross(up, norm)) * float(sizew);
    auto upx = glm::normalize(up) * float(sizeh);

    // LT
    output[0] = pos - rightx + upx;

    // LB
    output[1] = pos - rightx - upx;

    // RT
    output[2] = pos + rightx + upx;

    // RB
    output[3] = pos + rightx - upx;
}

ScoreGrade getTimingGrade(double absTime, double noteTime, const GameRules &rules) {
    if (isOverlapped(noteTime, rules.judgeTimeWindow.perfect, absTime, 0)) {
        return PF;
    } else if (isOverlapped(noteTime, rules.judgeTimeWindow.almost, absTime, 0)) {
        return AT;
    } else if (isOverlapped(noteTime, rules.judgeTimeWindow.good, absTime, 0)) {
        return AC;
    } else if (isOverlapped(noteTime, rules.judgeTimeWindow.range, absTime, 0)) {
        return TC;
    } else {
        return LT;
    }
}
