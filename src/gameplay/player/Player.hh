#ifndef GAMEPLAY_PLAYER_PLAYER
#define GAMEPLAY_PLAYER_PLAYER

class Player;

#include "./chars/CharList.hh"
#include "gameplay/base/Game.hh"
#include <string>
#include <memory>

enum SkillType {
    SK_PASSIVE,
    SK_AUX,
    SK_FINAL,
};

enum PlayerStat {
    NORMAL,
    DOWNED,
    KILLED
};

struct PlayerAssets {
    std::string auxTex, finalTex;
};

class Player {
public:
    virtual ~Player() = default;

    [[nodiscard]] unsigned int getUID() const;

    [[nodiscard]] unsigned int getPID() const;

    [[nodiscard]] std::string getPlayerName() const;

    [[nodiscard]] std::string getCharName() const;

    [[nodiscard]] std::string getSkillName(SkillType st) const;

    [[nodiscard]] PlayerStat getPlayerStat() const;

    [[nodiscard]] double getHealthRate() const;

    [[nodiscard]] double getShieldRate() const;

    // Try to damage the player
    virtual void damage(unsigned int amount, bool real);

    virtual void damage(unsigned int amount) {
        damage(amount, true);
    }

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
    [[nodiscard]] virtual double getSkillStat(SkillType st) const;

    // Apply modifiers to the game and update self status
    // The display value of some stat like HP and shield are automatically synced
    // However, changes like judge window must be done manually
    virtual void tick(Game &g);

    // Mark a skill as activated. It will be activated formally on the next tick.
    virtual void activateSkill(SkillType st);

    /**
     * @brief Generate a player instance.
     *
     * @note The pointer returned is allocated using `new` and the caller is fully
     * responsible for freeing the memory. The class `Player` has already defined a
     * virtual destructor, therefore, a simple `delete` will do the trick.
     *
     * @param ch The id of the character.
     * @param playerName The name of the player.
     * @param uid The user id of the account.
     * @param pid The player's position index.
     * @param isDummy Whether this player is not controller by user, but a copy of a remote teammate.
     * @return A pointer to the newly created player instance.
     */
    static std::shared_ptr<Player>
    createPlayer(CharID ch, const std::string &playerName, unsigned int uid, unsigned int pid, bool isDummy);

    // Gets skill name
    [[nodiscard]] virtual PlayerAssets getAssets() const;

protected:
    unsigned int health, maxHealth;
    unsigned int uid, pid;
    bool isDummy;
    std::string playerName, charName, auxName, finalName;
    unsigned int score; // TODO: reserved
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
