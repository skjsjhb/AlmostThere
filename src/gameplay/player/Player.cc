#include "Player.hh"

#include "./chars/CharAll.hh"
#include "spdlog/spdlog.h"

using namespace spdlog;

unsigned int
Player::getUID() const {
  return uid;
}

unsigned int
Player::getPID() const {
  return pid;
}

std::string
Player::getPlayerName() const {
  return playerName;
}

std::string
Player::getCharName() const {
  return charName;
}

std::string Player::getSkillName(SkillType st) const {
  switch (st) {
  case SK_AUX:return auxName;
  case SK_FINAL:return finalName;
  default:return "";
  }
}

PlayerStat Player::getPlayerStat() const {
  return stat;
}

unsigned int Player::getMaxHealth() const {
  return maxHealth;
}

unsigned int Player::getHealth() const {
  return health;
}

void Player::damage(unsigned int amount, bool real) {
  if (stat == KILLED) {
    return;
  }
  auto prevShield = shield;
  if (real) {
    prevShield = shield;
    shield = 0;
  }
  if (amount <= shield) {
    shield -= amount;
    return;
  }
  if (amount > shield) {
    shield = 0;
    auto rest = amount - shield;
    if (health > rest) {
      health -= rest;
    } else if (stat == NORMAL) {
      // Regeneration and set to unstable core
      health = maxHealth;
      stat = UNSTABLE;
    } else if (stat == UNSTABLE) {
      // Downed
      health = 0;
      stat = DOWNED;
    }
  }
  if (real) {
    shield = prevShield;
  }

}

void Player::heal(unsigned int amount) {
  health += amount;
  if (health > maxHealth) {
    health = maxHealth;
  }
}

void Player::chargeShield(unsigned int amount) {
  shield += amount;
  if (shield > maxShield) {
    shield = maxShield;
  }
}

void Player::chargeFinal(unsigned int amount) {
  finalChargedPt += amount;
  if (finalChargedPt > finalReqPt) {
    finalChargedPt = finalReqPt;
  }
}

double Player::getSkillStat(SkillType st) const {
  switch (st) {
  case SK_PASSIVE:return 2;
  case SK_AUX:
    if (nowTime < auxEnd) {
      return 2;
    } else {
      if (nowTime > auxNextReadyTime) {
        return 1;
      }
      return (nowTime - auxEnd) / (auxNextReadyTime - auxEnd);
    }
  case SK_FINAL:
    if (nowTime < finalEnd) {
      return 2;
    } else {
      return finalChargedPt / (double) finalReqPt;
    }
  }
  return -1;
}

void Player::tick(Game &g) {
  nowTime = g.mapTimer.getTime();
  // Dummy skill trigger reset
  if (auxShouldActivate) {
    auxShouldActivate = false;
  } else if (auxShouldDeactivate) {
    auxShouldDeactivate = false;
  }
  if (finalShouldActivate) {
    finalShouldActivate = false;
  } else if (finalShouldDeactivate) {
    finalShouldDeactivate = false;
  }
}

void Player::activateSkill(SkillType st) {
  auto sr = getSkillStat(st);
  if (sr != 1) {
    // Note ready
    return;
  }
  if (st == SK_AUX) {
    auxShouldActivate = true;
  }
  if (st == SK_FINAL) {
    finalShouldActivate = true;
  }
}

std::shared_ptr<Player> Player::createPlayer(CharID ch, const std::string &n, unsigned int u, unsigned int p, bool d) {
  if (d) {
    info("Creating DUMMY player with type " + std::to_string(ch));
  } else {
    info("Creating player '" + n + "' with type " + std::to_string(ch) + " and uid " + std::to_string(u));
  }
  std::shared_ptr<Player> px;
  switch (ch) {
  case NEKO:px = std::make_shared<NekoChar>();
    break;
  case DEFAULT:
  default:px = std::make_shared<DefaultChar>();
    break;
  }
  px->playerName = n;
  px->uid = u;
  px->pid = p;
  px->isDummy = d;
  return px;
}

PlayerAssets Player::getAssets() const {
  return {};
}

unsigned int Player::getShield() const {
  return shield;
}

unsigned int Player::getMaxShield() const {
  return maxShield;
}
