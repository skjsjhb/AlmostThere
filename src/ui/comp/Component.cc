#include "Component.hh"

#include "engine/virtual/Graphics.hh"
#include "util/Util.hh"
#include <glm/gtc/matrix_transform.hpp>
#include <utility>

static DrawContext defaultCtx = {
    .viewMat = glm::mat4(1),
    .projMat = glm::ortho<float>(0, 1600, 0, 900),
};

DrawContext &getDefaultDrawContext() {
  return defaultCtx;
}

void Component::computeLayout() { // NOLINT
  computeLayoutUnit(&layout);
  for (auto &d : children) {
    d->computeLayout();
  }
}

void Component::patchComponent(const std::shared_ptr<Component> &p,
                               const std::string &id,
                               std::list<std::shared_ptr<Component>> &&ch) {
  p->id = id;
  p->children = std::move(ch);
  for (auto &c : p->children) {
    c->parent = p; // Link self targets
    c->layout.parent = &p->layout; // Link layout targets
  }
}

Align inferAlignMethod(const std::string &al) {
  if (al == "end") {
    return Align::END;
  }
  if (al == "center") {
    return Align::CENTER;
  }
  return Align::BEGIN;
}

void applyLayoutParams(ComponentProps props, LayoutUnit &layout) {
  layout.dx = props[Props::X];
  layout.dy = props[Props::Y];
  layout.w = props[Props::Width];
  layout.h = props[Props::Height];
  auto xalignst = props[Props::XAlign];
  auto rt = splitStr(xalignst, ",");
  while (rt.size() < 2) {
    rt.emplace_back("");
  }
  layout.selfHorz = inferAlignMethod(rt[0]);
  layout.baseHorz = inferAlignMethod(rt[1]);
  auto yalignst = props[Props::YAlign];
  auto ht = splitStr(yalignst, ",");
  while (ht.size() < 2) {
    ht.emplace_back("");
  }
  layout.selfVert = inferAlignMethod(ht[0]);
  layout.baseVert = inferAlignMethod(ht[1]);
}
