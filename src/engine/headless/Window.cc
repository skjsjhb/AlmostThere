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
