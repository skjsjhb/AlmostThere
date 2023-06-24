#ifndef ALMOSTTHERE_SRC_GAMEPLAY_HUD_FLOATTEXT_HH
#define ALMOSTTHERE_SRC_GAMEPLAY_HUD_FLOATTEXT_HH

#include <string>
#include <glm/glm.hpp>
#include <utility>

class Game;

enum class FloatTextLevel {
  Normal,
  Warning,
  Error,
  Success
};

class FloatText {
public:
  explicit FloatText(Game &g, std::wstring t, float yPos, float sz, FloatTextLevel lv = FloatTextLevel::Normal);

  void draw();
protected:
  std::wstring text;
  float y, size; // X is automatically calculated
  double beginTime;
  Game &game;
  FloatTextLevel level;
};

#endif // ALMOSTTHERE_SRC_GAMEPLAY_HUD_FLOATTEXT_HH
