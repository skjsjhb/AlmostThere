#include "engine/virtual/Input.hh"
#include "engine/virtual/Window.hh"
#include "engine/virtual/UIHook.hh"
#include <GLFW/glfw3.h>
#include "spdlog/spdlog.h"

using namespace spdlog;

void vtPollEvents()
{
    glfwPollEvents();
}

static InputBuffer ibuf;

static glm::vec2 mousePos;
static int mouseCount = 0;

static void updateMouseStatus()
{
    ibuf.touchPoints.clear();
    if (mouseCount > 0)
    {
        ibuf.touchPoints.push_back(mousePos);
    }
    vtNotifyUIHooks(ibuf);
}

// On PC there is only one touch point.
static inline void _internalMousePosCallback(GLFWwindow *window, double x, double y)
{
    mousePos[0] = x;
    mousePos[1] = y;
    updateMouseStatus();
}

static void _internalMouseBtnCallback(GLFWwindow *window, int btn, int act, int mods)
{
    if (act == GLFW_RELEASE)
    {
        mouseCount--;
    }
    else if (act == GLFW_PRESS)
    {
        mouseCount++;
    }
    updateMouseStatus();
}

void vtSetupListeners()
{
    info("Setting up input listeners.");
    auto w = static_cast<GLFWwindow *>(vtGetWindow());
    glfwSetCursorPosCallback(w, _internalMousePosCallback);

    glfwSetMouseButtonCallback(w, _internalMouseBtnCallback);
}

const InputBuffer &vtGetInputBuffer()
{
    return ibuf;
}
