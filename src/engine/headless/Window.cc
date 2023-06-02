#include "engine/virtual/Window.hh"

void vtInitWindow() {}

typedef int FakeWindow;

static FakeWindow _internalWindow = 0;

void *vtGetWindow()
{
    return &_internalWindow;
}

void vtStopWindow() {}

bool vtWindowLoop()
{
    return false;
}

void vtGetWindowSize(int &x, int &y)
{
    x = 1600;
    y = 900;
}

void vtGetCoord(int sx, int sy, int &rx, int &ry)
{
    rx = sx;
    ry = sy;
}

void vtSetFPSCap(unsigned int fps) {}

bool vtShouldDraw() { return true; }

void vtDisplayFlip() {}

void vtSetTPSCap(unsigned int tps) {}
