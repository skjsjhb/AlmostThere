#include "Game.hh"

#include "gameplay/objs/Slot.hh"

void Game::runMainLoop()
{
    DrawContext globalCtx;
    while (status != DONE)
    {
        auto mapTimeNow = mapTimer.getTime();
        mapSchedule.notify(mapTimeNow);
        absSchedule.notify(absTimer.getTime());
        // InputManager.processEvents(events);
        // Handle events

        // Poll Events and process input
        // Tick notes
        for (auto &n : activeNotes)
        {
            n.tick(mapTimeNow);
        }
        // Perform judges
        for (auto &n : activeNotes)
        {
            n.draw(globalCtx);
        }
        // Draw notes
    }
}