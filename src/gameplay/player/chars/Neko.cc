#include "Neko.hh"

#define NEKO_AUX_CD 30
#define NEKO_AUX_DUR 6
#define NEKO_FINAL_DUR 25
#define NEKO_SHIELD_CHARGE_INT 1.0
#define NEKO_SHIELD_CHARGE_RATE 5

NekoChar::NekoChar() {
  charName = "NEKO";
  auxName = "NOTE SENSE";
  finalName = "NEKO UP!";
  finalReqPt = 40;
  finalChargedPt = 0;
  health = maxHealth = 100;
  shield = maxShield = 125;
}

void NekoChar::damage(unsigned int amount, bool real) {
  // Damage reduction
  Player::damage(static_cast<int>(amount * 0.85 + 0.5), real);
  // TODO: input latency reduction
}

void NekoChar::tick(Game &g) {
  // TODO: rebuild character module
}

PlayerAssets NekoChar::getAssets() const {
  return {.auxTex = "chars/neko/a", .finalTex = "chars/neko/final"};
}
