#include "engine/virtual/Input.hh"
#include "engine/virtual/Window.hh"
#include "engine/virtual/UIHook.hh"
#include <GLFW/glfw3.h>
#include "spdlog/spdlog.h"

using namespace spdlog;

static InputBuffer ibuf;

static glm::vec2 mousePos;
static int mouseCount = 0;

static void updateMouseStatus() {
    ibuf.touchPoints.clear();
    if (mouseCount > 0) {
        ibuf.touchPoints.push_back(mousePos);
    }
    vtNotifyUIHooks(ibuf);
}

// On PC there is only one touch point.
static inline void internalMousePosCallback(GLFWwindow *, double x, double y) {
    mousePos[0] = float(x);
    mousePos[1] = float(y);
    updateMouseStatus();
}

static void internalMouseBtnCallback(GLFWwindow *, int, int act, int) {
    if (act == GLFW_RELEASE) {
        mouseCount--;
    } else if (act == GLFW_PRESS) {
        mouseCount++;
    }
    updateMouseStatus();
}

void vtSetupListeners() {
    info("Setting up input listeners.");
    auto w = static_cast<GLFWwindow *>(vtGetWindow());
    glfwSetCursorPosCallback(w, internalMousePosCallback);

    glfwSetMouseButtonCallback(w, internalMouseBtnCallback);
}

const InputBuffer &vtGetInputBuffer() {
    return ibuf;
}
