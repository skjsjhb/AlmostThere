#include "HUDManager.hh"

#include "gameplay/base/Game.hh"
#include "gameplay/player/Player.hh"
#include <spdlog/spdlog.h>

void HUDManager::draw() {
  // Sync status
  if (game.players.size() < game.pid) {
    return;
  }
  auto &p = game.players[game.pid - 1];
  hp.setMaxHP(p->getMaxHealth());
  hp.setVariant(p->getPlayerStat() == NORMAL ? HP_NORMAL : HP_LOW);
  hp.setHP(p->getHealth(), true);

  shield.setShield(p->getShield());
  shield.setMaxShield(p->getMaxShield());
  score.setScore(game.score.exportScore());

  // Draw
  shield.draw();
  hp.draw();
  score.draw();
}
