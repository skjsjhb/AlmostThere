#include "Component.hh"

#include "engine/virtual/Graphics.hh"
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
