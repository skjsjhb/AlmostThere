#ifndef GAMEPLAY_BASE_GAME
#define GAMEPLAY_BASE_GAME

#include "gameplay/time/Timer.hh"
#include "gameplay/objs/Note.hh"
#include "gameplay/view/View.hh"
#include "gameplay/event/Schedule.hh"
#include "engine/virtual/Graphics.hh"
#include "gameplay/map/MapDef.hh"
#include "gameplay/map/MapLoad.hh"
#include "user/Account.hh"
#include <vector>
#include <string>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <list>
#include "gameplay/player/Player.hh"

enum GameStatus
{
    RUNNING, // Player is playing
    PAUSED,  // Paused, either resting, syncing data, or playing animation
    RESULT,  // Result screen
    DONE,    // Leaving
};

/**
 * @brief Generated objects from map.
 *
 * This class stores all objects used by the game, including their name map.
 */
struct GeneratedObjects
{
    /**
     * @brief Sorted pending objects buffer.
     */
    std::list<std::shared_ptr<TickObject>> bufferedObjects;

    /**
     * @brief Active objects for updating on each tick.
     */
    std::unordered_set<std::shared_ptr<TickObject>> activeObjects;

    /**
     * @brief Objects for name indexing.
     */
    std::map<std::string, std::weak_ptr<TickObject>> namedObjects;

    // This buffer is not used yet
    // std::set<std::shared_ptr<TickObject>> unloadedObjects;
};

class Game
{
public:
    void runMainLoop(); // Start the whole main loop, return when game completed
    void runOnce();     // Run the loop once, suitable for external calls
    void initGame(const std::string &mapId);

    /**
     * @brief Adds a player into game.
     *
     * This method is called after character selection has completed and the map has been
     * determined. The stat of the players will be initialized and the game is ready to begin.
     *
     * @note It's invalid to add two or more `ONLINE` or `LOCAL` account into the game. The active
     * account is unique and is setup for current player.
     *
     * @param account The account used for this player.
     * @param selectedChar The id of the character selected.
     */
    void addPlayer(const Account &account, CharID selectedChar);

    Timer mapTimer, absTimer;
    GameStatus status = RUNNING;
    View view;
    InputSet input;

    /**
     * @brief The id of the current active player.
     */
    int pid;
    // Schedule mapSchedule, absSchedule;
    std::vector<std::shared_ptr<Player>> players;
    GeneratedObjects objects;
    GameMap map;
    ScoreManager score;
    DrawContext drawContext;
};

#endif /* GAMEPLAY_BASE_GAME */
