#include "engine/virtual/Window.hh"

#include "TestTools.hh"
int main()
{
    vtInitWindow();
    WANT(vtGetWindow() != NULL);
    vtStopWindow();
    TEND;
}