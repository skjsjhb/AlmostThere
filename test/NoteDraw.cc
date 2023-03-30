#include "TestTools.hh"
#include "engine/virtual/Graphics.hh"
#include "engine/virtual/Window.hh"
#include "engine/virtual/Framework.hh"
#include "engine/virtual/Input.hh"
#include "gameplay/objs/Note.hh"

int main()
{
    vtInitWindow();

    // Create a note. Verified in NotePos.cc
    Tapu t;
    Slot s;
    Panel p;

    bool running = true;
    p.basePoint[0] = p.basePoint[1] = p.basePoint[2] = 0;
    p.normal[0] = p.normal[1] = 0;
    p.normal[2] = 1;
    p.up[0] = p.up[2] = 0;
    p.up[1] = 1;

    t.hitTime = 2;
    t.isFake = false;
    t.isVisible = true;
    t.targetSlot = &s;
    t.autoControl = true;
    t.keyCode = 87; // Key 'W'
    s.center[0] = s.center[1] = 0;
    s.center[2] = 0.01;
    s.normal[0] = s.normal[1] = 0;
    s.normal[2] = 1;
    s.up[1] = 1;
    s.up[0] = s.up[2] = 0;
    s.isVisible = true;
    DrawContext ctx;
    vec3 camPos = {0, -1, 1}, camDir = {0, 1, -1}, camUp = {0, 1, 1};
    ctx.cam.setState(camPos, camDir, camUp, glm_rad(90), 960.0 / 540);

    InputSet inputs;
    GameRules gr;
    gr.judgeValue = {
        .perfect = 300,
        .almost = 200,
        .accepted = 150,
        .medium = 100,
        .touched = 50,
        .lost = 0,
        .spaceNotesAmplifier = 2.0,
    };

    gr.judgeTime = {
        .range = 0.2,
        .good = 0.15,
        .almost = 0.08,
        .perfect = 0.04,
    };
    ScoreManager sm(gr);

    vtGraphicsInit();
    inputs.setupInputListeners();
    while (running)
    {
        inputs.pollInputEvents();
        auto time = 0.5 * vtGetTime();
        if (time > 3)
        {
            break;
        }

        ctx.polygons.clear();
        t.performJudge(time, inputs, sm);
        t.tick(time); // Test it faster
        t.draw(ctx);
        s.tick(time);
        s.draw(ctx);
        p.tick(time);
        p.draw(ctx);
        vtDraw(ctx);
        vtWindowLoop();
    }
    vtGraphicsCleanUp();

    // We cannot verify the output, so let's just verify that there are no errors
    WANT(vtGetGraphicsError() == 0);
    vtStopWindow();
    TEND;
}