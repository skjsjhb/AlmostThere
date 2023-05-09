#include "engine/virtual/Window.hh"
#include "engine/virtual/Graphics.hh"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "spdlog/spdlog.h"
using namespace spdlog;

static GLFWwindow *_internalWindow = NULL;

static void adjustFramebuffer(GLFWwindow *window, int x, int y)
{
    glViewport(0, 0, x, y);
    vtSetBufferSize(x, y);
}

static void acceptWindowClose(GLFWwindow *window)
{
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void vtInitWindow()
{
    info("Welcome to VT engine GLFW backend.");
    info("Initializing libraries.");
    glfwInit();
    // TODO: change according to settings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 16);
    info("GLFW: " + std::string(glfwGetVersionString()));
    info("GLAD: " + std::string(GLAD_GENERATOR_VERSION));
    info("Creating game window.");
    _internalWindow = glfwCreateWindow(1920, 1080, "Almost There", NULL, NULL);
    if (_internalWindow == NULL)
    {
        critical("Unable to create game window. Is GL library corrupted, or not supported in this context?");
        exit(1);
    }
    glfwMakeContextCurrent(_internalWindow);
    auto ver = gladLoadGL(glfwGetProcAddress);
    if (!ver)
    {
        critical("Unable to load GLAD loader. Current GLFW version might not be compatible with GLAD.");
        exit(1);
    }
    info("Window created.");
    info("OpenGL Context: " + std::to_string(GLAD_VERSION_MAJOR(ver)) + "." + std::to_string(GLAD_VERSION_MINOR(ver)));
    // Now GL functions are available
    glEnable(GL_MULTISAMPLE);
    // glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, 1920, 1080);

    // Setup framebuffer callback
    glfwSetFramebufferSizeCallback(_internalWindow, adjustFramebuffer);
    glfwSetWindowCloseCallback(_internalWindow, acceptWindowClose);
    vtSetBufferSize(1920, 1080); // Initial update
}

void *vtGetWindow()
{
    return _internalWindow;
}

void vtStopWindow()
{
    info("Closing game window.");
    glfwDestroyWindow(_internalWindow);
    glfwTerminate();
}

bool vtWindowLoop()
{
    glfwSwapBuffers(_internalWindow);
    glfwPollEvents();
    return glfwWindowShouldClose(_internalWindow);
}

void vtGetWindowSize(int &x, int &y)
{
    glfwGetWindowSize(_internalWindow, &x, &y);
}