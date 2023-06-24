#include "HUDManager.hh"

#include "FloatText.hh"
#include "event/Event.hh"
#include "gameplay/base/Game.hh"
#include "gameplay/player/PlayerEvents.hh"
#include "support/Locale.hh"
#include <spdlog/spdlog.h>

void HUDManager::draw() {
  // Sync status
  auto &p = game.player;

  auto hpiv = p->getHealthInfo();
  hp.setMaxHP(hpiv.maxHealth);
  hp.setVariant(hpiv.state == HealthState::Normal ? HP_NORMAL : HP_LOW);
  hp.setHP(hpiv.hp, true);

  shield.setShield(hpiv.shield);
  shield.setMaxShield(hpiv.maxShield);
  score.setScore(game.score.exportScore());

  // Draw
  shield.draw();
  hp.draw();
  score.draw();

  emHealth.draw(game.mapTimer.getTime(), game.drawList);
  emShield.draw(game.mapTimer.getTime(), game.drawList);
  emFlash.draw(game.mapTimer.getTime(), game.drawList);

  for (auto &t : floatText) {
    t.draw();
  }
}
void HUDManager::addEventListeners() {

  playerDamageEventHandler = addEventListener<PlayerDamageEvent>([this](PlayerDamageEvent &e) -> void {
    // Hit effect on damage
    if (e.getDamageType() == DamageType::Shield) {
      this->emShield.refresh(game.mapTimer.getTime());
    } else {
      this->emHealth.refresh(game.mapTimer.getTime());
    }
  });

  playerShieldBrokenEventHandler =
      addEventListener<PlayerShieldBreakEvent>([this](PlayerShieldBreakEvent &e) -> void {
        // Shield Broken
        putText(getLocaleText("HUD/ShieldBroken/Title"),
                getLocaleText("HUD/ShieldBroken/SubTitle"),
                FloatTextLevel::Warning);
      });

  playerCoreUnstableEventHandler =
      addEventListener<PlayerCoreUnstableEvent>([this](PlayerCoreUnstableEvent &e) -> void {
        // Shield Broken
        putText(getLocaleText("HUD/CoreUnstable/Title"),
                getLocaleText("HUD/CoreUnstable/SubTitle"),
                FloatTextLevel::Error);
      });

  playerDieEventHandler = addEventListener<PlayerDieEvent>([this](PlayerDieEvent &e) -> void {
    floatText.clear();
    emFlash.refresh(game.mapTimer.getTime());
  });
}

void HUDManager::removeEventListeners() const {
  removeEventListener<PlayerDamageEvent>(playerDamageEventHandler);
  removeEventListener<PlayerShieldBreakEvent>(playerShieldBrokenEventHandler);
  removeEventListener<PlayerCoreUnstableEvent>(playerCoreUnstableEventHandler);
  removeEventListener<PlayerDieEvent>(playerDieEventHandler);
}

#define TITLE_SIZE 0.5
#define SUBTITLE_SIZE 0.25
#define TITLE_Y 700
#define SUBTITLE_Y 650

void HUDManager::putText(const std::wstring &title, const std::wstring &subtitle, FloatTextLevel lv) {
  floatText.clear();
  floatText.emplace_back(game, title, TITLE_Y, TITLE_SIZE, lv);
  floatText.emplace_back(game, subtitle, SUBTITLE_Y, SUBTITLE_SIZE, lv);
}
