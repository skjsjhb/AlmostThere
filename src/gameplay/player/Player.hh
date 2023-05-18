#ifndef GAMEPLAY_PLAYER_PLAYER
#define GAMEPLAY_PLAYER_PLAYER

#include "gameplay/score/Score.hh"

class Player;

#include "./chars/CharList.hh"
#include "gameplay/base/Game.hh"
#include <string>

enum SkillType
{
    SK_PASSIVE,
    SK_AUX,
    SK_FINAL,
};

enum PlayerStat
{
    NORMAL,
    DOWNED,
    KILLED
};

struct PlayerAssets
{
    std::string auxTex, finalTex;
};

class Player
{
public:
    virtual ~Player() = default;

    unsigned int getUID();
    unsigned int getPID();

    std::string getPlayerName();
    std::string getCharName();
    std::string getSkillName(SkillType st);

    PlayerStat getPlayerStat();

    double getHealthRate();
    double getShieldRate();

    // Try to damage the player
    virtual void damage(unsigned int amount, bool real = false);

    // Try to heal the player
    virtual void heal(unsigned int amount);

    // Try to charge shield
    virtual void chargeShield(unsigned int amount);

    // Try to charge the final
    virtual void chargeFinal(unsigned int amount);

    /**
     * @brief Get skill cooldown status.
     *
     * @param st The skill to query
     * @param mapTime Current map time. Used for time-based cooldowns.
     * @return A number represents the status. -1 for disabled, 2 for active (some
     * long-lasting finals), 0-1 for cooldown status.
     */
    virtual double getSkillStat(SkillType st);

    // Get score
    virtual ScoreManager &getScoreManager();

    // Apply modifiers to the game and update self status
    // The display value of some stat like HP and shield are automatically synced
    // However, changes like judge window must be done manually
    virtual void tick(Game &g);

    // Mark a skill as activated. It will be activated formally on the next tick.
    virtual void activateSkill(SkillType st);

    // Generate a player instance. The object is created using `new`.
    static Player *createPlayer(CharID ch, const std::string &playerName, unsigned int uid, unsigned int pid, bool isDummy);

    // Gets skill name
    virtual PlayerAssets getAssets();

protected:
    unsigned int health, maxHealth;
    unsigned int uid, pid;
    bool isDummy;
    std::string playerName, charName, auxName, finalName;
    ScoreManager score;
    PlayerStat stat = NORMAL;
    unsigned shield, maxShield;
    bool auxShouldActivate = false, finalShouldActivate = false, auxActive = false;
    bool auxShouldDeactivate = false, finalShouldDeactivate = false, finalActive = false;
    double auxEnd = 0, auxNextReadyTime = 0;
    unsigned int finalChargedPt = 0, finalReqPt;
    double finalEnd = 0;
    double nowTime = 0;
};

#endif /* GAMEPLAY_PLAYER_PLAYER */
