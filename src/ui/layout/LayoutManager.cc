#include "LayoutManager.hh"
#include <set>
#include <spdlog/spdlog.h>

static Component rootComp = {
        .dx = 0,
        .dy = 0,
        .w = 1600,
        .h = 900,
        .baseHorz = AL_BEGIN,
        .baseVert = AL_BEGIN,
        .selfHorz = AL_BEGIN,
        .selfVert = AL_BEGIN,
        .parent = nullptr,
        .result = {
                .ok = true,
                .w = 1600,
                .h = 900,
        },
};

void LayoutManager::addComponent(Component *c) {
    c->result.ok = false;
    if (c->parent == nullptr) {
        c->parent = &rootComp;
    }
    components.push_back(c);
}

static bool analyzeCircular(const Component *c) {
    std::set<Component *> chain;
    auto cc = c->parent;
    while (cc != nullptr) {
        if (chain.contains(cc)) {
            return true;
        }
        chain.insert(cc);
        cc = cc->parent;
    }
    return false;
}

static double getRelativeValue(const Length &l, double parent) {
    if (l.mode == LM_ABSOLUTE) {
        return l.value;
    } else if (l.mode == LM_PERCENT) {
        return l.value * parent;
    }
    return 0;
}

static double getAlignValue(double base, double length, ComponentAlign al) {
    if (al == AL_BEGIN) {
        return base;
    } else if (al == AL_CENTER) {
        return base - length / 2;
    } else if (al == AL_END) {
        return base - length;
    }
    return 0;
}

static double getAlignBase(double base, double baseLength, double chi, ComponentAlign al) {
    if (al == AL_BEGIN) {
        return base + chi;
    } else if (al == AL_CENTER) {
        return base + baseLength / 2 + chi;
    } else if (al == AL_END) {
        return base + baseLength + chi;
    }
    return 0;
}

static void solveComponent(Component *c) { // NOLINT
    if (c->parent != nullptr && !c->result.ok) {
        if (!c->parent->result.ok) {
            solveComponent(c->parent);
        }
        auto &rs = c->result;
        rs.ok = true;
        rs.w = getRelativeValue(c->w, c->parent->result.w);
        rs.h = getRelativeValue(c->h, c->parent->result.h);
        auto cdx = getRelativeValue(c->dx, c->parent->result.w);
        auto cdy = getRelativeValue(c->dy, c->parent->result.h);
        cdx = getAlignValue(cdx, rs.w, c->selfHorz);
        cdy = getAlignValue(cdy, rs.h, c->selfVert);
        rs.x = getAlignBase(c->parent->result.x, c->parent->result.w, cdx, c->baseHorz);
        rs.y = getAlignBase(c->parent->result.y, c->parent->result.h, cdy, c->baseVert);
    }
}

void LayoutManager::compile() {
    if (components.empty()) {
        return;
    }
    for (auto c: components) {
        if (analyzeCircular(c)) {
            // Circular
            spdlog::warn("Circular UI components detected. Unable to do layout.");
            return;
        }
    }
    for (auto &c: components) {
        solveComponent(c);
    }
}
