#ifndef GAMEPLAY_CORE_PLAYERSTAT
#define GAMEPLAY_CORE_PLAYERSTAT

#include "gameplay/score/Score.hh"

#include <string>

struct Shield
{
    unsigned int maxHP, curHP;
    double phyRate, eleRate; // Absorb rate
    unsigned int nextExtRequire;
    unsigned int rechargeRate;
};

// Status of a player
class PlayerStat
{
    unsigned int uid;
    std::string playerName;
    ScoreManager score;
    Shield shield;
    unsigned int health, maxHealth;
    unsigned int magic, maxMagic;
    unsigned int regenerationRate, magicReplenishRate;
};

#endif /* GAMEPLAY_CORE_PLAYERSTAT */
