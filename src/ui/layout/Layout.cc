#include "Layout.hh"
#include <set>
#include <spdlog/spdlog.h>
#include <stdio.h>

static LayoutUnit rootUnit = {
    .dx =  0,
    .dy = 0,
    .w = 1600,
    .h =  900,
    .baseHorz = Align::BEGIN,
    .baseVert = Align::BEGIN,
    .selfHorz = Align::BEGIN,
    .selfVert = Align::BEGIN,
    .parent = nullptr,
    .result = {
        .ok = true,
        .w = 1600,
        .h = 900,
    },
};

static bool analyzeCircular(const LayoutUnit *c) {
  std::set<LayoutUnit *> chain;
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

static double getAlignValue(double base, double length, Align al) {
  if (al == Align::BEGIN) {
    return base;
  } else if (al == Align::CENTER) {
    return base - length / 2;
  } else if (al == Align::END) {
    return base - length;
  }
  return 0;
}

static double getAlignBase(double base, double baseLength, double chi, Align al) {
  if (al == Align::BEGIN) {
    return base + chi;
  } else if (al == Align::CENTER) {
    return base + baseLength / 2 + chi;
  } else if (al == Align::END) {
    return base + baseLength + chi;
  }
  return 0;
}

static void solveUnit(LayoutUnit *c) { // NOLINT
  if (c->parent != nullptr && !c->result.ok) {
    if (!c->parent->result.ok) {
      solveUnit(c->parent);
    }
    auto &rs = c->result;
    rs.ok = true;
    rs.w = c->w.get(c->parent->result.w);
    rs.h = c->h.get(c->parent->result.h);
    auto cdx = c->dx.get(c->parent->result.w);
    auto cdy = c->dy.get(c->parent->result.h);
    cdx = getAlignValue(cdx, rs.w, c->selfHorz);
    cdy = getAlignValue(cdy, rs.h, c->selfVert);
    rs.x = getAlignBase(c->parent->result.x, c->parent->result.w, cdx, c->baseHorz);
    rs.y = getAlignBase(c->parent->result.y, c->parent->result.h, cdy, c->baseVert);
  }
}

void computeLayoutUnit(LayoutUnit *c) {
  if (c->result.ok) {
    return;
  }
  if (c->parent == nullptr) {
    c->parent = getRootUnit();
  }
  if (analyzeCircular(c)) {
    // Circular
    spdlog::warn("Circular UI units detected. Unable to do layout.");
    return;
  }
  solveUnit(c);
}

Length::Length(std::string src) {
  if (src.empty()) {
    value = 0;
    mode = LengthMode::ABSOLUTE;
    return;
  }
  if (src.ends_with('%')) {
    src.pop_back();
    value = std::stod(src) / 100; // Percentage division
    mode = LengthMode::PERCENT;
  } else {
    value = std::stod(src);
    mode = LengthMode::ABSOLUTE;
  }
}
double Length::get(double parentValue) const {
  if (mode == LengthMode::ABSOLUTE) {
    return value;
  } else if (mode == LengthMode::PERCENT) {
    return value * parentValue;
  }
  return 0;
}

LayoutUnit *getRootUnit() {
  return &rootUnit;
}