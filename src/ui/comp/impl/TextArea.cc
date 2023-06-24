#include "TextArea.hh"
#include "util/Util.hh"
#include <string>

TextArea::TextArea(ComponentProps props) : Component(props) {
  applyLayoutParams(props, layout);
  size = std::stof(props[Props::Size]);
  text = unicode2wstring(props[Props::Text]);
  color = props[Props::Color];
  font = props[Props::Font];

  if (layout.w.value == 0) {
    // Infer width
    layout.w.mode = LengthMode::ABSOLUTE;
    for (auto &t : text) {
      float w, h;
      vtGetCharSize(font, t, w, h);
      layout.w.value += w * size;
    }
  }
}

#define LINE_HEIGHT_SCALE 1.5f

void TextArea::draw(DrawList &d) {
  DrawParam p = {
      .shader = "hud/text",
      .transparent = true,
      .ctx = getDefaultDrawContext()
  };

  float chw, chh;
  vtGetCharSize(font, L'\u3007', chw, chh); // Using a CJK font to align line spacing
  if (chh == 0) {
    return; // Unable to draw
  }
  float lineHeight = chh * LINE_HEIGHT_SCALE * size;
  auto maxw = layout.result.w;
  double posx, posy = layout.result.y + layout.result.h - lineHeight;
  double deltaW = 0;

  for (auto &c : text) {
    if (c == '\n') {
      posy -= lineHeight;
      deltaW = 0;
      continue;
    }
    float cw, ch;
    vtGetCharSize(font, c, cw, ch);
    cw *= size;
    ch *= size;
    if (deltaW + cw > maxw) {
      posy -= lineHeight;
      deltaW = cw;
      posx = layout.result.x;
    } else {
      posx = layout.result.x + deltaW;
      deltaW += cw;
    }
    std::wstring s;
    s += c;
    DisplayText t({posx, posy}, size, s, parseColorStr(color), font, p);
    d.add(t);
  }
}
