#include "engine/virtual/Window.hh"
#include "engine/virtual/Graphics.hh"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "spdlog/spdlog.h"
#include <iostream>
using namespace spdlog;

static GLFWwindow *_internalWindow = NULL;
static int wx = 1920, wy = 1080;

// In dev env we use a 1920x1080 window to develop, so we need an extra scale.
#define IMPL_DEV_EXT_SCALE 1600.0 / 1920.0

#define ENABLE_FPS_COUNT

static double scaleFactor = 1920.0 / 1600.0;

static void adjustFramebuffer(GLFWwindow *window, int x, int y)
{
    // Normalize it to 16:9
    auto ratio = x / (double)y;
    int dx, dy, cx, cy;
    if (ratio > 16 / 9.0)
    {
        // Clip X
        cx = y / 9.0 * 16;
        cy = y;
        dx = (int)(x - cx) / 2;
    }
    else if (ratio < 16 / 9.0)
    {
        // Clip Y
        cy = x / 16.0 * 9;
        cx = x;
        dy = (int)(y - cy) / 2;
    }
    scaleFactor = cx / 1600.0;
    glViewport(dx, dy, cx, cy);
    vtSetBufferSize(cx, cy);
    wx = cx;
    wy = cy;
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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 16);

    info("GLFW: " + std::string(glfwGetVersionString()));
    info("GLAD: " + std::string(GLAD_GENERATOR_VERSION));
    info("Creating game window.");
    _internalWindow = glfwCreateWindow(wx, wy, "Almost There", NULL, NULL);
    if (_internalWindow == NULL)
    {
        critical("Unable to create game window. Is GL library corrupted, or not supported in this context?");
        exit(1);
    }
    glfwMakeContextCurrent(_internalWindow);
    glfwSwapInterval(0); // Disable vsync
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

    glEnable(GL_DEPTH_CLAMP);
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

#ifdef ENABLE_FPS_COUNT
static double lastTime = 0;
static unsigned int frames = 0;
#define FPS_COUNT_PERIOD 30
#endif

static double spf = 0;
static double lastFrameTime = 0;

bool vtWindowLoop()
{
    ++frames;
    auto currentTime = glfwGetTime();
    if (currentTime - lastTime >= FPS_COUNT_PERIOD)
    {
        auto fps = int(frames / (currentTime - lastTime));
        info("FPS: " + std::to_string(fps));
        lastTime = currentTime;
        frames = 0;
    }
    glfwPollEvents();
    glfwSwapBuffers(_internalWindow);
    return glfwWindowShouldClose(_internalWindow);
}

void vtGetWindowSize(int &x, int &y)
{
    glfwGetWindowSize(_internalWindow, &x, &y);
}

#define VT_STDW_W 1600.0
#define VT_STDW_H 900.0

void vtGetCoord(int sx, int sy, int &rx, int &ry)
{
    rx = sx * wx / VT_STDW_W;
    ry = sy * wy / VT_STDW_H;
    while (rx < 0)
    {
        rx += wx;
    }
    while (rx > wx)
    {
        rx -= wx;
    }
    while (ry < 0)
    {
        ry += wy;
    }
    while (ry > wy)
    {
        ry -= wy;
    }
}

void vtDeCoord(int rx, int ry, int &sx, int &sy)
{
    sx = rx * VT_STDW_W / wx;
    sy = ry * VT_STDW_H / wy;
}

double vtGetScaleFactor()
{
    return scaleFactor * IMPL_DEV_EXT_SCALE;
}

void vtSetFPSCap(unsigned int fps)
{
    spf = 1.0 / fps;
}

bool vtShouldDraw()
{
    return (glfwGetTime() - lastFrameTime) >= spf;
}
