#ifndef GAMEPLAY_BASE_GAME
#define GAMEPLAY_BASE_GAME

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

class Game;

#include "gameplay/player/Player.hh"

enum GameStatus
{
    RUNNING, // Player is playing
    PAUSED,  // Paused, either resting, syncing data, or playing animation
    RESULT,  // Result screen
    DONE,    // Leaving
};

class Game
{
public:
    void runMainLoop(); // Start the whole main loop, return when game completed
    void runOnce();     // Run the loop once, suitable for external calls
    void initGame(const std::string &mapId);
    Timer mapTimer, absTimer;
    GameStatus status = RUNNING;
    World world;
    InputSet input;
    int cPlayerID;
    std::vector<Player> players;
    Schedule mapSchedule, absSchedule;
    std::set<AbstractNote *> activeNotes, doneNotes;
    std::set<Slot *> activeSlots, doneSlots;
    std::list<AbstractNote *> pendingNotes;
    std::list<Slot *> pendingSlots;
    std::set<Camera *> cameras;

    GameMap map;
    ScoreManager score;
    DrawContext drawContext;
};

#endif /* GAMEPLAY_BASE_GAME */
