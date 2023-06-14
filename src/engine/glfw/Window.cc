#include "engine/virtual/Window.hh"
#include "engine/virtual/Graphics.hh"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "spdlog/spdlog.h"
#include "engine/virtual/Input.hh"
#include <string>
#include <thread>
#include <chrono>
#include <iostream>

using namespace spdlog;

static GLFWwindow *internalWindow = nullptr;
static int wx = 1920, wy = 1080;

// In dev env we use a 1920x1080 window to develop, so we need an extra scale.

#define ENABLE_FPS_COUNT

static void adjustFramebuffer(GLFWwindow *, int x, int y) {
  // Normalize it to 16:9
  auto ratio = x / (double) y;
  int dx, dy, cx, cy;
  if (ratio > 16 / 9.0) {
    // Clip X
    cx = int(y / 9.0 * 16);
    cy = y;
    dx = (int) (x - cx) / 2;
  } else if (ratio < 16 / 9.0) {
    // Clip Y
    cy = int(x / 16.0 * 9);
    cx = x;
    dy = (int) (y - cy) / 2;
  }
  glViewport(dx, dy, cx, cy);
  wx = cx;
  wy = cy;
}

static void acceptWindowClose(GLFWwindow *window) {
  glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void vtInitWindow() {
  info("Welcome to Almost There VMC Turbo Engine! (GLFW Backend)");
  info("Initializing libraries.");
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  info("GLFW: " + std::string(glfwGetVersionString()));
  info("GLAD: " + std::string(GLAD_GENERATOR_VERSION));
  info("Creating game window.");
  internalWindow = glfwCreateWindow(wx, wy, "Almost There", nullptr, nullptr);
  if (internalWindow == nullptr) {
    critical("Unable to create game window. Is GL library corrupted, or not supported in this context?");
    exit(1);
  }
  glfwMakeContextCurrent(internalWindow);
  glfwSwapInterval(0); // Disable vsync
  auto ver = gladLoadGL(glfwGetProcAddress);
  if (!ver) {
    critical("Unable to load GLAD loader. Current GLFW version might not be compatible with GLAD.");
    exit(1);
  }
  info("Created window with OpenGL context: " + std::to_string(GLAD_VERSION_MAJOR(ver)) + "."
           + std::to_string(GLAD_VERSION_MINOR(ver)));

  glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_DEPTH_CLAMP);
  glViewport(0, 0, 1920, 1080);

  // Setup framebuffer callback
  glfwSetFramebufferSizeCallback(internalWindow, adjustFramebuffer);
  glfwSetWindowCloseCallback(internalWindow, acceptWindowClose);

}

void *vtGetWindow() {
  return internalWindow;
}

void vtStopWindow() {
  info("Closing game window.");
  glfwDestroyWindow(internalWindow);
  glfwTerminate();
}

#ifdef ENABLE_FPS_COUNT
static double lastTime = 0;
static unsigned int frames = 0;
#define FPS_COUNT_PERIOD 10
#endif

static double spf = 0, spt = 0;
static double lastFrameTime = 0;

static double lastTickTime = 0;

#define TPS_MINIMUM_COUNT 500

static void exSleep(double dt) {
  static constexpr std::chrono::duration<double> a(0);
  std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
  while (std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start).count() < dt) {
    std::this_thread::sleep_for(a);
  }
}

bool vtWindowLoop() {
  ++frames;
  auto currentTime = glfwGetTime();

  if (currentTime - lastTime >= FPS_COUNT_PERIOD) {
    auto tps = int(frames / (currentTime - lastTime));
    info("TPS: " + std::to_string(tps));
    if (tps < TPS_MINIMUM_COUNT) {
      warn("Low TPS (<" + std::to_string(TPS_MINIMUM_COUNT) + ") detected. Game tick might delay.");
    }
    lastTime = currentTime;
    frames = 0;
  }

  auto tmfs = currentTime - lastTickTime;
  if (tmfs > 0) {
    exSleep(spt - tmfs);
  }
  lastTickTime = glfwGetTime();
  glfwPollEvents();
  return glfwWindowShouldClose(internalWindow);
}

void vtGetWindowSize(int &x, int &y) {
  glfwGetWindowSize(internalWindow, &x, &y);
}

#define VT_STDW_W 1600.0
#define VT_STDW_H 900.0

void vtGetCoord(int sx, int sy, int &rx, int &ry) {
  rx = int(sx * wx / VT_STDW_W);
  ry = int(sy * wy / VT_STDW_H);
  while (rx < 0) {
    rx += wx;
  }
  while (rx > wx) {
    rx -= wx;
  }
  while (ry < 0) {
    ry += wy;
  }
  while (ry > wy) {
    ry -= wy;
  }
}

void vtDeCoord(int rx, int ry, int &sx, int &sy) {
  sx = int(rx * VT_STDW_W / wx);
  sy = VT_STDW_H - int(ry * VT_STDW_H / wy); // Reverse-Y
}

void vtDisplayFlip() {
  glfwSwapBuffers(internalWindow);
  lastFrameTime = glfwGetTime();
}

void vtSetTPSCap(unsigned int tps) {
  if (tps < TPS_MINIMUM_COUNT) {
    warn("Target TPS (" + std::to_string(tps) + ") is too low and therefore not accepted.");
    return;
  }
  info("TPS limit set to " + std::to_string(tps));
  spt = 1.0 / tps;
}

void vtSetFPSCap(unsigned int fps) {
  info("FPS limit set to " + std::to_string(fps));
  spf = 1.0 / fps;
}

bool vtShouldDraw() {
  return glfwGetTime() - lastFrameTime >= spf;
}
