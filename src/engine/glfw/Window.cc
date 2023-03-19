#include "engine/virtual/Window.hh"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static GLFWwindow *_internalWindow = NULL;

void vtInitWindow()
{
    glfwInit();
    // TODO: change according to settings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    _internalWindow = glfwCreateWindow(960, 540, "Almost There", NULL, NULL);
    glfwMakeContextCurrent(_internalWindow);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    glViewport(0, 0, 960, 540);
}

void *vtGetWindow()
{
    return _internalWindow;
}

void vtStopWindow()
{
    glfwDestroyWindow(_internalWindow);
    glfwTerminate();
}

void vtWindowLoop()
{
    glfwSwapBuffers(_internalWindow);
    glfwPollEvents();
}
