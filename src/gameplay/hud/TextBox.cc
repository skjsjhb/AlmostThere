#include "TextBox.hh"

// Right & Top align
#define TEXT_BOX_XEND 1530
#define TEXT_BOX_YEND 740
#define TEXT_BOX_MARGINX 15
#define TEXT_BOX_MARGINY 6
#define TEXT_BOX_GAP 50
#define TEXT_BOX_CHAR_SIZE 0.2
#define TEXT_BOX_XSHIFT 5

#define TEXT_BOX_FONT "mono"

void TextBox::draw() {
  // Calculate text size
  float txw = 0, txh = 0;
  for (auto &c : text) {
    float aw = 0, ah = 0;
    vtGetCharSize(TEXT_BOX_FONT, c, aw, ah);
    if (ah > txh) {
      txh = ah;
    }
    txw += aw;
  }
  txw *= TEXT_BOX_CHAR_SIZE;
  txh *= TEXT_BOX_CHAR_SIZE;
  float xbegin = TEXT_BOX_XEND - 2 * TEXT_BOX_MARGINX - txw;
  float xend = TEXT_BOX_XEND;
  float yend = TEXT_BOX_YEND - float(positionIndex) * TEXT_BOX_GAP;
  float ybegin = yend - 2 * TEXT_BOX_MARGINY - txh;
  DrawParam p = {
      .shader = "hud/color-rect",
      .transparent = true,
      .args = {color[0], color[1], color[2], color[3]},
      .ctx = game.ctxUI,
  };
  Rect
      bgr({{xbegin + TEXT_BOX_XSHIFT, yend, 0}},
          {{xbegin - TEXT_BOX_XSHIFT, ybegin, 0}},
          {{xend + TEXT_BOX_XSHIFT, yend, 0}},
          {{xend - TEXT_BOX_XSHIFT, ybegin, 0}},
          p);
  DrawParam pt = {
      .transparent = true,
      .ctx = game.ctxUI,
  };
  DisplayText
      t(glm::vec2(xbegin + TEXT_BOX_MARGINX, ybegin + TEXT_BOX_MARGINY),
        TEXT_BOX_CHAR_SIZE,
        text,
        glm::vec4(1, 1, 1, 1),
        TEXT_BOX_FONT,
        pt, true);
  game.drawList.add(bgr);
  game.drawList.add(t);
}
