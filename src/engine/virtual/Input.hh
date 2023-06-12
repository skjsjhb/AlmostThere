#ifndef ENGINE_VIRTUAL_INPUT
#define ENGINE_VIRTUAL_INPUT

#include "event/Event.hh"

#include <set>
#include <list>
#include <glm/glm.hpp>

/**
 * @brief Input point struct.
 *
 * On PC and mobile we have different input methods. We describe both mouse and touch points as \c InputPoint.
 *
 * This is designed as an substitution of the original \c InputBuffer system.
 */
struct InputPoint {
  unsigned long id = 0; // Unique id to identify this point
  double x = 0, y = 0; // Corrected position
  bool pressed = false; // Whether this point is pressed. On mobile this is always false.
};

/**
 * @brief Base event class, triggered when input btn state changed.
 */
class InputEvent : Event {
public:
  explicit InputEvent(const InputPoint &i) : inputPoint(i) {}

  /**
   * @brief Get the related input point of this event.
   * @return
   */
  [[nodiscard]] const InputPoint &getInputPoint() const { return inputPoint; };
protected:
  const InputPoint &inputPoint;
};

/**
 * @brief Triggered when an input point is pressed.
 */
class InputPressEvent : InputEvent {
EVENT_HANDLERS(InputPressEvent)
EVENT_DISPATCH

public:
  using InputEvent::InputEvent;
};

/**
 * @brief Triggered when an input point is released.
 */
class InputReleaseEvent : InputEvent {
EVENT_HANDLERS(InputReleaseEvent)
EVENT_DISPATCH

public:
  using InputEvent::InputEvent;
};

/**
 * @brief Triggered when an input point is clicked. That is, pressed and released, while not changing its position.
 *
 * @note This should be used for UI clicks, but not in gameplay, as players are likely to reposition while clicking.
 */
class InputClickEvent : InputEvent {
EVENT_HANDLERS(InputClickEvent)
EVENT_DISPATCH

public:
  using InputEvent::InputEvent;
};

/**
 * @brief Triggered when the input points moves, regardless of its btn state.
 */
class InputMoveEvent : InputEvent {
EVENT_HANDLERS(InputMoveEvent)
EVENT_DISPATCH

public:
  InputMoveEvent(const InputPoint &i, double x, double y) : InputEvent(i), fromX(i.x), fromY(i.y), toX(x), toY(y) {};

  [[nodiscard]] double getToX() const { return toX; }
  [[nodiscard]] double getToY() const { return toY; }
  [[nodiscard]] double getFromX() const { return fromX; }
  [[nodiscard]] double getFromY() const { return fromY; }
protected:
  double fromX, fromY;

protected:
  double toX, toY;
};

/**
 * @brief Triggered when the input points moves while pressed. Useful cor creating dragging handlers.
 */
class InputDragEvent : InputMoveEvent {
EVENT_HANDLERS(InputDragEvent)
EVENT_MIXED_DISPATCH(InputMoveEvent)

public:
  using InputMoveEvent::InputMoveEvent;
};

/**
 * @brief Init the input system. Events will be dispatched when triggered at once.
 *
 * @note For any engine requires manually event polling (e.g. GLFW, SDL), the event polling are done in the window loop,
 * see \c vtWindowLoop for details.
 */
extern void vtInitInput();

/**
 * @brief Get all input points currently present.
 *
 * @note Unstable - This is implemented for old judge system without event handlers. This will be replaced soon.
 * @return A list of read-only pointers to the existing input points.
 */
extern std::list<const InputPoint *> vtGetInputPoints();

#endif /* ENGINE_VIRTUAL_INPUT */
