#include "engine/virtual/Window.hh"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static GLFWwindow *_internalWindow = NULL;

static void adjustFramebuffer(GLFWwindow *window, int x, int y)
{
    glViewport(0, 0, x, y);
}

void vtInitWindow()
{
    glfwInit();
    // TODO: change according to settings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    _internalWindow = glfwCreateWindow(1920, 1080, "Almost There", NULL, NULL);
    glfwMakeContextCurrent(_internalWindow);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

    // Now GL functions are available
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
    glViewport(0, 0, 1920, 1080);

    // Setup framebuffer callback
    glfwSetFramebufferSizeCallback(_internalWindow, adjustFramebuffer);
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
