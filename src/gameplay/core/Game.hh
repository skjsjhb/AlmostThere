#ifndef GAMEPLAY_CORE_GAME
#define GAMEPLAY_CORE_GAME

#include "PlayerStat.hh"
#include "gameplay/time/Timer.hh"
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
    Timer mapTimer;
    GameStatus status = RUNNING;
    unsigned int cPlayerID; // Current player
    std::vector<PlayerStat> playerStats;
};

#endif /* GAMEPLAY_CORE_GAME */
