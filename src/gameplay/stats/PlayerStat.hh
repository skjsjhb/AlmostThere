#ifndef GAMEPLAY_STAT_PLAYERSTAT
#define GAMEPLAY_STAT_PLAYERSTAT

#include "Score.hh"
#include "HealthData.hh"

#include <string>

// Status of a player
class PlayerStat
{
    unsigned int uid;
    std::string playerName;
    ScoreManager score;
    HealthData health;
};

#endif /* GAMEPLAY_STAT_PLAYERSTAT */
