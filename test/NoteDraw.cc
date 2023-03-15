#include "TestTools.hh"
#include "engine/virtual/Graphics.hh"
#include "engine/virtual/Window.hh"
#include "engine/virtual/Framework.hh"
#include "gameplay/objs/Note.hh"

int main()
{
    vtInitWindow();

    // Create a note. Verified in NotePos.cc
    Tapu t;
    Slot s;
    DrawContext ctx;
    t.hitTime = 2;
    t.isFake = false;
    t.isVisible = true;
    t.targetSlot = &s;
    t.autoControl = true;
    s.center[0] = s.center[1] = s.center[2] = 0;
    s.normal[0] = s.normal[1] = 0;
    s.normal[2] = 1;
    s.up[1] = 1;
    s.up[0] = s.up[2] = 0;
    t.tick(1.8);
    t.draw(ctx);
    vec3 camPos = {0, 0, 2}, camDir = {0, 0, -1}, camUp = {0, 1, 0};
    ctx.cam.setState(camPos, camDir, camUp, glm_rad(75), 960.0 / 540);
    vtDraw(ctx);
    vtWindowLoop();
    // We cannot verify the output, so let's just verify that there are no errors
    WANT(vtGetGraphicsError() == 0);
    vtStopWindow();
    TEND;
}