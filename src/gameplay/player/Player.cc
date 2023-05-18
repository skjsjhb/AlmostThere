#include "Player.hh"

#include "./chars/CharAll.hh"
#include "spdlog/spdlog.h"

using namespace spdlog;

unsigned int
Player::getUID()
{
    return uid;
}

unsigned int
Player::getPID()
{
    return pid;
}

std::string
Player::getPlayerName()
{
    return playerName;
}

std::string
Player::getCharName()
{
    return charName;
}

std::string Player::getSkillName(SkillType st)
{
    switch (st)
    {
    case SK_AUX:
        return auxName;
    case SK_FINAL:
        return finalName;
    default:
        return "";
    }
}

PlayerStat
Player::getPlayerStat()
{
    return stat;
}

double Player::getHealthRate()
{
    return health / (double)maxHealth;
}

double Player::getShieldRate()
{
    return shield / (double)maxShield;
}

void Player::damage(unsigned int amount, bool real)
{
    if (stat == KILLED)
    {
        return;
    }
    if (real || stat == DOWNED)
    {
        if (health > amount)
        {
            health -= amount;
        }
        else
        {
            health = 0;
            stat = DOWNED;
        }
    }
    else
    {
        if (shield > amount)
        {
            shield -= amount;
        }
        else
        {
            auto extra = amount - shield;
            damage(extra, true);
        }
    }
}

void Player::heal(unsigned int amount)
{
    health += amount;
    if (health > maxHealth)
    {
        health = maxHealth;
    }
}

void Player::chargeShield(unsigned int amount)
{
    shield += amount;
    if (shield > maxShield)
    {
        shield = maxShield;
    }
}

void Player::chargeFinal(unsigned int amount)
{
    finalChargedPt += amount;
    if (finalChargedPt > finalReqPt)
    {
        finalChargedPt = finalReqPt;
    }
}

double Player::getSkillStat(SkillType st)
{
    switch (st)
    {
    case SK_PASSIVE:
        return 2;
    case SK_AUX:
        if (nowTime < auxEnd)
        {
            return 2;
        }
        else
        {
            if (nowTime > auxNextReadyTime)
            {
                return 1;
            }
            return (nowTime - auxEnd) / (auxNextReadyTime - auxEnd);
        }
    case SK_FINAL:
        if (nowTime < finalEnd)
        {
            return 2;
        }
        else
        {
            return finalChargedPt / (double)finalReqPt;
        }
    }
    return -1;
}

ScoreManager &Player::getScoreManager()
{
    return score;
}

void Player::tick(Game &g)
{
    nowTime = g.mapTimer.getTime();
    // Dummy skill trigger reset
    if (auxShouldActivate)
    {
        auxShouldActivate = false;
    }
    else if (auxShouldDeactivate)
    {
        auxShouldDeactivate = false;
    }
    if (finalShouldActivate)
    {
        finalShouldActivate = false;
    }
    else if (finalShouldDeactivate)
    {
        finalShouldDeactivate = false;
    }
    return;
}

void Player::activateSkill(SkillType st)
{
    auto sr = getSkillStat(st);
    if (sr != 1)
    {
        // Note ready
        return;
    };
    if (st == SK_AUX)
    {
        auxShouldActivate = true;
    }
    if (st == SK_FINAL)
    {
        finalShouldActivate = true;
    }
    return;
}

Player *Player::createPlayer(CharID ch, const std::string &n, unsigned int u, unsigned int p, bool d)
{
    if (d)
    {
        info("Creating DUMMY player with type " + std::to_string(ch));
    }
    else
    {
        info("Creating player '" + n + "' with type " + std::to_string(ch) + " and uid " + std::to_string(u));
    }
    Player *px;
    switch (ch)
    {
    case NEKO:
        px = new NekoChar;
        break;
    case DEFAULT:
    default:
        px = new DefaultChar;
        break;
    }
    px->playerName = n;
    px->uid = u;
    px->pid = p;
    px->isDummy = d;
    return px;
}

PlayerAssets Player::getAssets()
{
    return {};
}