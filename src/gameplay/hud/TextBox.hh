#ifndef ALMOSTTHERE_SRC_GAMEPLAY_HUD_TEXTBOX_HH
#define ALMOSTTHERE_SRC_GAMEPLAY_HUD_TEXTBOX_HH

#include <string>
#include <glm/glm.hpp>
#include <utility>
#include "gameplay/base/Game.hh"

class TextBox {
public:
  explicit TextBox(Game &g, const glm::vec4 &c, double lifeTimeIn)
      : game(g), color(c), lifeTime(lifeTimeIn), beginTime(g.mapTimer.getTime()) {}

  void setText(const std::wstring &t) { text = t; }
  void setIndex(unsigned int i) { positionIndex = i; }

  void draw();

protected:
  Game &game;
  std::wstring text;
  glm::vec4 color;
  double lifeTime, beginTime;
  unsigned int positionIndex = 0;
};

#endif // ALMOSTTHERE_SRC_GAMEPLAY_HUD_TEXTBOX_HH
