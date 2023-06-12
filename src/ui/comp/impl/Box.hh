#ifndef ALMOSTTHERE_SRC_UI_COMP_IMPL_BOX_HH
#define ALMOSTTHERE_SRC_UI_COMP_IMPL_BOX_HH

#include "../Component.hh"
#include <string>

class Box : public Component {
public:
  Box(ComponentProps props); // NOLINT

  void draw(DrawList &d) override;
protected:
  std::string background;

  /**
   * @brief Whether this box is visible.
   *
   * Boxes are designed to be insivible. If there is any necessity to display them, the props should set \c visible to
   * \c "1" .
   */
  bool visible = false;
};

#endif // ALMOSTTHERE_SRC_UI_COMP_IMPL_BOX_HH
