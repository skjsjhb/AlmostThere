#include "TestTools.hh"
#include "engine/virtual/Graphics.hh"
#include "engine/virtual/Window.hh"
#include "engine/virtual/Framework.hh"
#include "engine/virtual/Input.hh"
#include "gameplay/objs/Note.hh"
#include "gameplay/objs/Mask.hh"

int main()
{
    vtInitWindow();

    // Create a note. Verified in NotePos.cc
    Tapu t;
    Shizuku k;
    Puresu r;
    Hoshi h;
    Hashi a;
    Slot s;
    Mask m;

    vec3 aColor = {0, 1, 1};
    for (int i = 0; i < 3; i++)
    {
        m.color[i] = aColor[i];
    }
    m.beginRad = 0.9;

    bool running = true;

    t.hitTime = 2;
    t.isFake = false;
    t.isVisible = true;
    t.targetSlot = &s;
    t.autoControl = true;
    t.keyCode = 87; // Key 'W'

    k.hitTime = 4;
    k.isFake = false;
    k.isVisible = true;
    k.targetSlot = &s;
    k.autoControl = true;
    k.keyCode = 87;

    r.isFake = false;
    r.isVisible = true;
    r.targetSlot = &s;
    r.autoControl = true;
    r.keyCode = 87;
    r.hitTime = 6;
    r.absLength = 2;

    h.hitTime = 10;
    h.isFake = false;
    h.isVisible = true;
    h.targetSlot = &s;
    h.autoControl = true;
    h.keyCode = 87;

    a.hitTime = 12;
    a.isFake = false;
    a.isVisible = true;
    a.targetSlot = &s;
    a.autoControl = true;
    a.keyCode = 87;
    a.absLength = 2;

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
    double time = 0;
    while (running)
    {
        inputs.pollInputEvents();
        time = vtGetTime();
        if (time > 16)
        {
            break;
        }
        if (time < 2)
        {
            s.center[0] = -4;
        }
        else if (time > 2 && time < 4)
        {
            s.center[0] = -2;
        }
        else if (time > 4 && time < 8)
        {
            s.center[0] = 0;
        }
        else if (time > 8 && time < 10)
        {
            s.center[0] = 2;
        }
        else if (time > 10 && time < 14)
        {
            s.center[0] = 4;
        }
        ctx.polygons.clear();

        glm_vec3_normalize(s.up);
        s.tick(time);
        t.performJudge(time, inputs, sm);
        t.tick(time);
        k.performJudge(time, inputs, sm);
        k.tick(time);
        r.performJudge(time, inputs, sm);
        r.tick(time);
        h.performJudge(time, inputs, sm);
        h.tick(time);
        a.performJudge(time, inputs, sm);
        a.tick(time);

        m.tick(time);

        s.draw(ctx);
        r.draw(ctx);
        a.draw(ctx);
        t.draw(ctx);
        k.draw(ctx);
        h.draw(ctx);
        m.draw(ctx);
        vtDraw(ctx);
        vtWindowLoop();
    }
    vtGraphicsCleanUp();

    // We cannot verify the output, so let's just verify that there are no errors
    WANT(vtGetGraphicsError() == 0);
    vtStopWindow();
    TEND;
}