#include "engine/virtual/Input.hh"
#include "engine/virtual/Window.hh"

#include <GLFW/glfw3.h>
#include "spdlog/spdlog.h"

using namespace spdlog;


// Input event handlers def
EVENT_HANDLERS_BODY(InputPressEvent)
EVENT_HANDLERS_BODY(InputReleaseEvent)
EVENT_HANDLERS_BODY(InputMoveEvent)
EVENT_HANDLERS_BODY(InputDragEvent)
EVENT_HANDLERS_BODY(InputClickEvent)

static InputPoint cursor = {.id = 1}; // On PC there is only one input point.

static unsigned int btnCount = 0;

static bool cursorMovedDuringPress = true;

static void mouseBtnCallback(GLFWwindow *, int, int act, int) {

  if (act == GLFW_RELEASE) {
    --btnCount;
    if (btnCount == 0) {
      // Released
      cursor.pressed = false;
      InputReleaseEvent rel(cursor);
      rel.dispatch();
      if (!cursorMovedDuringPress) {
        InputClickEvent cli(cursor);
        cli.dispatch();
        cursorMovedDuringPress = true;
      }
    }
  } else if (act == GLFW_PRESS) {
    if (btnCount == 0) {
      // Pressed
      cursorMovedDuringPress = false;
      cursor.pressed = true;
      InputPressEvent pre(cursor);
      pre.dispatch();
    }
    ++btnCount;
  }
}

static void cursorPosCallback(GLFWwindow *, double ex, double ey) {
  int x, y;
  vtDeCoord(int(ex), int(ey), x, y); // Coords correction
  if (cursor.pressed) {
    cursorMovedDuringPress = true;
  }
  // Trigger events
  if (cursor.pressed) {
    InputDragEvent e(cursor, x, y);
    e.dispatch();
  } else {
    InputMoveEvent e(cursor, x, y);
    e.dispatch();
  }

  // Update status
  cursor.x = x;
  cursor.y = y;
}

void vtInitInput() {
  info("Initializing input system.");
  auto w = static_cast<GLFWwindow *>(vtGetWindow());
  glfwSetCursorPosCallback(w, cursorPosCallback);
  glfwSetMouseButtonCallback(w, mouseBtnCallback);
}

extern std::list<const InputPoint *> vtGetInputPoints() {
  return {&cursor};
}
