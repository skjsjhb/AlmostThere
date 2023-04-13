#ifndef GAMEPLAY_CORE_GAME
#define GAMEPLAY_CORE_GAME

#include "PlayerStat.hh"
#include "gameplay/time/Timer.hh"
#include "gameplay/objs/Note.hh"
#include "gameplay/objs/World.hh"
#include "gameplay/event/Schedule.hh"
#include "engine/virtual/Graphics.hh"
#include <vector>

enum GameStatus
{
    RUNNING,  // Player is playing
    DOWNED,   // Downed, waiting for recovery
    PAUSED,   // Paused
    FAILED,   // Game failed, can choose to quit now
    FINISHED, // Finished locally, waiting for other players
    RESULT,   // All players have finished
    DONE,     // Game finished, leaving loop
};

class Game
{
public:
    void runMainLoop(); // Start the whole main loop, return when game completed

protected:
    Timer mapTimer, absTimer; // One is for map playing, one is for global animations
    GameStatus status = RUNNING;
    World world;            // Virtual world definition
    unsigned int cPlayerID; // Current player
    std::vector<PlayerStat> playerStats;
    Schedule mapSchedule, absSchedule;
    std::list<AbstractNote> activeNotes;
    // std::list<UIComponent> uiComponents;
    std::list<Slot> slots;
};

#endif /* GAMEPLAY_CORE_GAME */
