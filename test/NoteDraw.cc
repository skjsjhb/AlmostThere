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
    Puresu r;
    Hoshi h;
    Hashi a;

    bool running = true;
    r.isFake = false;
    r.isVisible = true;
    r.targetSlot = &s;
    r.autoControl = true;
    r.keyCode = 87;
    r.basePosition[0] = r.basePosition[1] = r.basePosition[2] = 0;
    r.hitTime = 4;
    r.absLength = 2;

    t.hitTime = 2;
    t.isFake = false;
    t.isVisible = true;
    t.targetSlot = &s;
    t.autoControl = true;
    t.keyCode = 87; // Key 'W'

    a.hitTime = 2;
    a.isFake = false;
    a.isVisible = true;
    a.targetSlot = &s;
    a.autoControl = true;
    a.keyCode = 87;
    a.basePosition[0] = a.basePosition[1] = a.basePosition[2] = 0;
    a.absLength = a.length = 0.5;

    h.hitTime = 2;
    h.isFake = false;
    h.isVisible = true;
    h.targetSlot = &s;
    h.autoControl = true;
    h.keyCode = 87;

    k.hitTime = 3;
    k.isFake = false;
    k.isVisible = true;
    k.targetSlot = &s;
    k.autoControl = true;
    k.keyCode = 87;

    s.center[0] = s.center[1] = 0;
    s.center[2] = 0;
    s.normal[0] = s.normal[1] = 0;
    s.normal[2] = 1;
    s.up[1] = 1;
    s.up[0] = s.up[2] = 0;
    s.isVisible = true;
    DrawContext ctx;
    vec3 camPos = {0, -6, 9}, camDir = {0, 0.8, -1}, camUp = {0, 1, 0.8};
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
        .allowBreak = 0.04,
    };
    ScoreManager sm(gr);

    vtGraphicsInit();
    inputs.setupInputListeners();
    unsigned int i = 0;
    double time = 0;
    while (running)
    {
        inputs.pollInputEvents();
        time = vtGetTime();
        a.basePosition[2] = 8 - vtGetTime();
        if (time > 4)
        {
            //  break;
        }
        ctx.polygons.clear();

        // p.tick(time);
        s.up[0] = sin(time);
        s.up[1] = cos(time);
        s.up[2] = 0;

        // h.basePosition[1] = 0;
        // h.basePosition[0] = 0;
        // h.basePosition[2] = 2;
        glm_vec3_normalize(s.up);
        s.tick(time);
        // t.performJudge(time, inputs, sm);
        // t.tick(time);
        // k.performJudge(time, inputs, sm);
        // k.tick(time);
        // r.performJudge(time, inputs, sm);
        // r.tick(time);
        // h.performJudge(time, inputs, sm);
        // h.tick(time);

        // p.draw(ctx);
        // s.draw(ctx);
        //      r.draw(ctx);
        a.draw(ctx);
        //        t.draw(ctx);
        //  k.draw(ctx);
        // h.draw(ctx);
        vtDraw(ctx);
        vtWindowLoop();
    }
    vtGraphicsCleanUp();

    // We cannot verify the output, so let's just verify that there are no errors
    WANT(vtGetGraphicsError() == 0);
    vtStopWindow();
    TEND;
}