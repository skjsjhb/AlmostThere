#ifndef GAMEPLAY_BASE_GAME
#define GAMEPLAY_BASE_GAME

#include "gameplay/player/Player.hh"
#include "gameplay/time/Timer.hh"
#include "gameplay/objs/Note.hh"
#include "gameplay/objs/World.hh"
#include "gameplay/event/Schedule.hh"
#include "engine/virtual/Graphics.hh"
#include "gameplay/map/MapDef.hh"
#include "gameplay/map/MapLoad.hh"
#include <vector>
#include <string>
#include <set>

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
    void runOnce();     // Run the loop once, suitable for external calls
    void initGame(const std::string &mapId);

protected:
    Timer mapTimer, absTimer; // One is for map playing, one is for global animations
    GameStatus status = RUNNING;
    World world; // Virtual world definition
    InputSet input;
    int cPlayerID; // Current player
    std::vector<Player> players;
    Schedule mapSchedule, absSchedule;
    std::set<AbstractNote *> activeNotes, doneNotes;
    std::set<Slot *> activeSlots, doneSlots;
    std::list<AbstractNote *> pendingNotes;
    std::list<Slot *> pendingSlots;
    std::set<Camera *> cameras;

    GameMap map;
    ScoreManager score;
    // std::list<UIComponent> uiComponents;
};

#endif /* GAMEPLAY_BASE_GAME */
