#include "Button.hh"
#include "engine/virtual/Graphics.hh"
#include "util/Util.hh"

EVENT_HANDLERS_BODY(ButtonClickEvent)

Button::Button(ComponentProps props) : Component(props) {
  applyLayoutParams(props, layout);
  color = props[Props::Color];
  background = props[Props::Background];
  if (background.empty()) {
    background = "ui/button";
  }
  addListeners();
}

Button::~Button() noexcept {
  removeListeners();
}

void Button::draw(DrawList &d) {
  auto c = getDefaultDrawContext();
  auto statArg = static_cast<float>(state);
  auto colvec = parseColorStr(color);
  DrawParam p = {
      .shader = "ui/button",
      .texture = background,
      .transparent = true,
      .args = {statArg, colvec[0], colvec[1], colvec[2], colvec[3]},
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

  for (auto &cc : children) {
    cc->draw(d);
  }
}

void Button::addListeners() {
  // TODO: This is a PC-only impl, on other platforms this needs to be changed.
  lsMove =
      addEventListener<InputMoveEvent>([this](InputMoveEvent &e) -> void {
        if (this->state != ButtonState::Disabled) {
          if (this->isInBound(e.getToX(), e.getToY())) {
            if (this->state != ButtonState::Pressed) {
              this->state = ButtonState::Hover;
            }
          } else {
            this->state = ButtonState::Default;
          }
        }
      });

  lsPress =
      addEventListener<InputPressEvent>([this](InputPressEvent &e) -> void {
        if (this->state != ButtonState::Disabled) {
          if (this->isInBound(e.getInputPoint().x, e.getInputPoint().y)) {
            this->state = ButtonState::Pressed;
          }
        }
      });

  lsRelease =
      addEventListener<InputReleaseEvent>([this](InputReleaseEvent &e) -> void {
        if (this->state != ButtonState::Disabled) {
          if (this->state == ButtonState::Pressed) { // Was pressed, fire click event
            auto be = ButtonClickEvent(id);
            be.dispatch();
          }
          if (isInBound(e.getInputPoint().x, e.getInputPoint().y)) {
            this->state = ButtonState::Hover;
          } else {
            this->state = ButtonState::Default;
          }
        }
      });
}

void Button::removeListeners() const {
  removeEventListener<InputMoveEvent>(lsMove);
  removeEventListener<InputPressEvent>(lsPress);
  removeEventListener<InputReleaseEvent>(lsRelease);
}

bool Button::isInBound(double x, double y) {
  return layout.result.x <= x && x <= (layout.result.x + layout.result.w) && layout.result.y <= y
      && y <= (layout.result.y + layout.result.h);
}
