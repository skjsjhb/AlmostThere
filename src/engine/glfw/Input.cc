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

static InputSet *_activeInputSet = NULL;

void vtSetActiveInputSet(InputSet *input)
{
    _activeInputSet = input;
}

static vec2 mousePos;
static int mouseCount = 0;

static void updateMouseStatus()
{
    if (_activeInputSet != NULL)
    {
        _activeInputSet->touchPoints.clear();
        if (mouseCount > 0)
        {
            _activeInputSet->touchPoints.insert(std::to_array(mousePos));
        }
        vtNotifyUIHooks(*_activeInputSet);
    }
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

void vtSetupKeyListener()
{
    info("Setting up input listeners.");
    auto w = static_cast<GLFWwindow *>(vtGetWindow());
    glfwSetCursorPosCallback(w, _internalMousePosCallback);
    glfwSetMouseButtonCallback(w, _internalMouseBtnCallback);
}
