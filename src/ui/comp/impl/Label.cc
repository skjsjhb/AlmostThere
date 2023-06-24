#include "Label.hh"
#include "util/Util.hh"
#include <string>

Label::Label(ComponentProps props) : Component(props) {
  applyLayoutParams(props, layout);
  size = std::stof(props[Props::Size]);
  text = unicode2wstring(props[Props::Text]);
  font = props[Props::Font];
  color = props[Props::Color];

  if (layout.w.value == 0 || layout.h.value == 0) {
    bool correctW = false, correctH = false;
    // Infer size
    if (layout.w.value == 0) {
      layout.w.mode = LengthMode::ABSOLUTE;
      correctW = true;
    }
    if (layout.h.value == 0) {
      layout.h.mode = LengthMode::ABSOLUTE;
      correctH = true;
    }
    float maxh = -1;
    for (auto &t : text) {
      float w, h;
      vtGetCharSize(font, t, w, h);
      if (h > maxh) {
        maxh = h;
      }
      if (correctW) {
        layout.w.value += w * size;
      }
    }
    if (correctH) {
      layout.h.value = maxh * size;
    }
  }
}

void Label::draw(DrawList &d) {
  DrawParam p = {
      .shader = "hud/text",
      .transparent = true,
      .ctx = getDefaultDrawContext()
  };
  DisplayText t({layout.result.x, layout.result.y}, float(size), text, parseColorStr(color), font, p);
  d.add(t);
}

