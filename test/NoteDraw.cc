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
    Shizuku k;
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

    k.hitTime = 3;
    k.isFake = false;
    k.isVisible = true;
    k.targetSlot = &s;
    k.autoControl = true;
    k.keyCode = 87;

    s.center[0] = s.center[1] = 0;
    s.center[2] = 0.01;
    s.normal[0] = s.normal[1] = 0;
    s.normal[2] = 1;
    s.up[1] = 1;
    s.up[0] = s.up[2] = 0;
    s.isVisible = true;
    DrawContext ctx;
    vec3 camPos = {0, 0, 10}, camDir = {0, 0, -1}, camUp = {0, 1, 0};
    ctx.cam.setState(camPos, camDir, camUp, glm_rad(60), 1920.0 / 1080);

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
    unsigned int i = 0;
    while (running)
    {
        inputs.pollInputEvents();
        auto time = (++i) / 100;
        if (time > 4) // 300 Ticks
        {
            break;
        }
        ctx.polygons.clear();

        p.tick(time);
        s.tick(time);
        t.performJudge(time, inputs, sm);
        t.tick(time);
        k.performJudge(time, inputs, sm);
        k.tick(time);

        p.draw(ctx);
        s.draw(ctx);
        t.draw(ctx);
        k.draw(ctx);

        vtDraw(ctx);
        vtWindowLoop();
    }
    vtGraphicsCleanUp();

    // We cannot verify the output, so let's just verify that there are no errors
    WANT(vtGetGraphicsError() == 0);
    vtStopWindow();
    TEND;
}