#ifndef ALMOSTTHERE_SRC_UI_COMP_IMPL_BUTTON_HH
#define ALMOSTTHERE_SRC_UI_COMP_IMPL_BUTTON_HH

#include <utility>

#include "engine/virtual/Input.hh"
#include "event/Event.hh"
#include "ui/comp/Component.hh"

enum class ButtonState {
  Default,
  Disabled,
  Hover,
  Pressed
};

class ButtonClickEvent : public Event {

EVENT_HANDLERS(ButtonClickEvent)
EVENT_DISPATCH
public:
  explicit ButtonClickEvent(std::string i) : id(std::move(i)) {};

protected:
  std::string id;
};

class Button : public Component {
public:
  explicit Button(ComponentProps props);
  ~Button() noexcept override;
  void draw(DrawList &d) override;
protected:
  void addListeners(); // Add input event listeners
  void removeListeners() const;
  bool isInBound(double x, double y);
  ButtonState state = ButtonState::Default;
  std::string background, color;
  unsigned int lsMove = 0, lsPress = 0, lsRelease = 0;

};

#endif // ALMOSTTHERE_SRC_UI_COMP_IMPL_BUTTON_HH
