#ifndef GAMEPLAY_STATS_GAME
#define GAMEPLAY_STATS_GAME

#include "PlayerStat.hh"
#include <vector>

enum GameStatus
{
    ACTIVE, // Player is playing
    DOWNED, // Downed, waiting for recovery
    PAUSED, // Paused
    FAILED, // Game failed, can choose to quit now
    DONE,   // Finished locally, waiting for other players
    RESULT, // All players have finished
};

class Game
{
public:
    void runMainLoop(); // Start the whole main loop, return when game completed

protected:
    double absTime, mapTimeline;
    unsigned int cPlayerID; // Current player
    std::vector<PlayerStat> playerStats;
};

#endif /* GAMEPLAY_STATS_GAME */
