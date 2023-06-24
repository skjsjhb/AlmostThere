#ifndef ALMOSTTHERE_HUDMANAGER_HH
#define ALMOSTTHERE_HUDMANAGER_HH

#include "HPBar.hh"
#include "ShieldBar.hh"
#include "ScoreDisplay.hh"
#include "gameplay/hud/EffectMask.hh"
#include "FloatText.hh"

class Game;
class DrawList;

class HUDManager {
public:
  explicit HUDManager(Game &g) : game(g), hp(g), shield(g), score(g),
                                 emShield("hud/effect-mask", "hud/shield-effect", EffectMaskVariant::PULSE),
                                 emHealth("hud/effect-mask", "hud/blood-effect", EffectMaskVariant::PULSE),
                                 emFlash("hud/effect-mask", "hud/flash-effect", EffectMaskVariant::GROW) {
    addEventListeners();
  }

  void draw();

  void addEventListeners();
  void removeEventListeners() const;

  ~HUDManager() noexcept { removeEventListeners(); };

  /**
   * @brief Put a pair of text using float text widget.
   * @param title,subtitle The content to display.
   * @param level The level of the text.
   */
  void putText(const std::wstring &title, const std::wstring &subtitle, FloatTextLevel level);

protected:
  Game &game;
  HPBar hp;
  ShieldBar shield;
  ScoreDisplay score;
  EffectMask emShield, emHealth, emFlash;
  std::list<FloatText> floatText;
  unsigned int playerDamageEventHandler = 0,
      playerShieldBrokenEventHandler = 0,
      playerCoreUnstableEventHandler = 0,
      playerDieEventHandler = 0;
};

#endif // ALMOSTTHERE_HUDMANAGER_HH
