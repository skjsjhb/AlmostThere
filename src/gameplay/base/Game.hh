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
#include <memory>
#include <string>
#include <unordered_set>
#include <set>
#include <vector>
#include "engine/virtual/Input.hh"
#include "gameplay/hud/HUDManager.hh"
#include "gameplay/player/Player.hh"

class Player;

enum GameStatus {
  RUNNING, // Player is playing
  PAUSED,  // Paused, either resting, syncing data, or playing animation
  DONE,    // Completed
  FAILED, // Player died or map error
};

struct AudioStat {
  unsigned int bgmBuf = 0;
  bool bgmPlaying = false;
};

class Game final {
public:
  Game() : hudManager(*this), ambient(*this) {}
  ~Game();
  void runMainLoop(); // Start the whole main loop, return when game completed
  void runOnce();     // Run the loop once, suitable for external calls
  void initGame(const std::string &mapId);
  void setPlayer(CharID ch);

  Timer mapTimer, absTimer;
  GameStatus status = RUNNING;
  View view;

  std::shared_ptr<Player> player;

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
