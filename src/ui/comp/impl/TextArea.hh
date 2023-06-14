#ifndef ALMOSTTHERE_SRC_UI_COMP_IMPL_TEXTAREA_HH
#define ALMOSTTHERE_SRC_UI_COMP_IMPL_TEXTAREA_HH

#include "engine/virtual/Graphics.hh"
#include "ui/comp/Component.hh"

/**
 * @brief Text area component.
 *
 * Text area is such a component which renders a batch of text, breaking lines accordingly (LF or width exceeded). The
 * width passed to it is guaranteed (to not to overflow), but the height is not.
 */
class TextArea : public Component {
public:
  explicit TextArea(ComponentProps props);
  void draw(DrawList &d) override;
protected:
  std::wstring text;
  float size;
  std::string color;
};

#endif // ALMOSTTHERE_SRC_UI_COMP_IMPL_TEXTAREA_HH
