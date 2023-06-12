#include "Box.hh"
#include "engine/virtual/Graphics.hh"

Box::Box(ComponentProps props) : Component(props) {
  layout.w = props[Props::Width];
  layout.h = props[Props::Height];
  layout.dx = props[Props::X];
  layout.dy = props[Props::Y];
  background = props[Props::Background];
  visible = props[Props::Visible] == "1";
}

void Box::draw(DrawList &d) {
  if (visible) {
    auto c = getDefaultDrawContext();
    DrawParam p = {
        .shader = "ui/box",
        .texture = background,
        .ctx = c,
    };
    auto x = layout.result.x;
    auto y = layout.result.y;
    auto w = layout.result.w;
    auto h = layout.result.h;

    Rect r({{x, y + h, 0}, {0, 1}},
           {{x, y, 0}, {0, 0}},
           {{x + w, y + h, 0}, {1, 1}},
           {{x + w, y, 0}, {1, 0}}, p);
    d.add(r);
  }
  // Call child draw method
  for (auto &cd : children) {
    cd->draw(d);
  }
}
