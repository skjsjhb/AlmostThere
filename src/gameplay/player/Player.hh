#ifndef GAMEPLAY_BASE_PLAYER
#define GAMEPLAY_BASE_PLAYER

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
struct Player
{

    unsigned int uid;
    std::string playerName, charName;
    ScoreManager score;
    Shield shield;
    unsigned int health, maxHealth;
    unsigned int magic, maxMagic;
    unsigned int regenerationRate, magicReplenishRate;
};

#endif /* GAMEPLAY_BASE_PLAYER */
