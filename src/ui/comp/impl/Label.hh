#ifndef ALMOSTTHERE_SRC_UI_COMP_IMPL_LABEL_HH
#define ALMOSTTHERE_SRC_UI_COMP_IMPL_LABEL_HH

#include "../Component.hh"
#include "engine/virtual/Graphics.hh"
#include <string>

class Label : public Component {
public:
  explicit Label(ComponentProps props);

  void draw(DrawList &d) override;
protected:
  std::wstring text;
  double size;
  std::string color, font;
};

#endif // ALMOSTTHERE_SRC_UI_COMP_IMPL_LABEL_HH
