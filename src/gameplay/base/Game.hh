#ifndef GAMEPLAY_BASE_GAME
#define GAMEPLAY_BASE_GAME

#include "engine/virtual/Audio.hh"
#include "engine/virtual/Graphics.hh"
#include "gameplay/ambient/AmbientManager.hh"
#include "gameplay/player/chars/CharList.hh"
#include "gameplay/rules/GameRules.hh"
#include "gameplay/score/ScoreRecord.hh"
#include "gameplay/time/Timer.hh"
#include "gameplay/view/View.hh"
#include "gameplay/map/MapGen.hh"
#include <list>
#include <map>
#include <string>
#include <unordered_set>
#include <set>
#include <vector>
#include "engine/virtual/Input.hh"
#include "gameplay/hud/HUDManager.hh"

class Player;

class Account;

enum GameStatus {
  RUNNING, // Player is playing
  PAUSED,  // Paused, either resting, syncing data, or playing animation
  DONE,    // Leaving
};

struct AudioStat {
  unsigned int bgmBuf = 0;
  bool bgmPlaying = false;
};

class Game final {
public:
  Game() : hudManager(*this), ambient(*this) {}

  void runMainLoop(); // Start the whole main loop, return when game completed
  void runOnce();     // Run the loop once, suitable for external calls
  void initGame(const std::string &mapId);

  /**
   * @brief Adds a player into game.
   *
   * This method is called after character selection has completed and the map
   * has been determined. The stat of the players will be initialized and the
   * game is ready to begin.
   *
   * @note It's invalid to add two or more \c ONLINE or \c LOCAL account into the
   * game. The active account is unique and is setup for current player.
   *
   * @param account The account used for this player.
   * @param selectedChar The id of the character selected.
   */
  void addPlayer(const Account &account, CharID selectedChar);

  Timer mapTimer, absTimer;
  GameStatus status = RUNNING;
  View view;

  /**
   * @brief The id of the current active player.
   */
  int pid = -1;
  std::vector<std::shared_ptr<Player>> players;

  MapData mapData;
  decltype(mapData.content.objects.begin()) mapObjectPtr;

  // Internal variable to process only active objects
  std::set<std::shared_ptr<TickObject>> activeObjects;

  /**
   * @brief The score of current player.
   */
  ScoreRecords score;
  AudioStat audio;
  GameRules rules;
  HUDManager hudManager;
  std::list<const InputPoint *> inputBuf;
  DrawContext ctx3D{}, ctxUI{};
  DrawList drawList;
  AmbientManager ambient;
};

#endif /* GAMEPLAY_BASE_GAME */
