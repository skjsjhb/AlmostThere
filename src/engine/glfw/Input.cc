#include "engine/virtual/Input.hh"
#include "engine/virtual/Window.hh"
#include <GLFW/glfw3.h>

void vtPollEvents()
{
    glfwPollEvents();
}

static InputSet *_activeInputSet = NULL;

void vtSetActiveInputSet(InputSet *input)
{
    _activeInputSet = input;
}

void _internalKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (_activeInputSet != NULL)
    {
        // Scancode might be better
        _activeInputSet->keyInfo[scancode] = action == GLFW_PRESS ? 1 : 0;
    }
}

void vtSetupKeyListener()
{
    glfwSetKeyCallback(static_cast<GLFWwindow *>(vtGetWindow()), _internalKeyCallback);
}
