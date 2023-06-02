#ifndef GAMEPLAY_RULES_GAMERULES
#define GAMEPLAY_RULES_GAMERULES

struct JudgeTimeWindow
{
    double range = 0.1, good = 0.08, almost = 0.04, perfect = 0.02, allowBreak;
};

// Generated game rules
struct GameRules
{
    JudgeTimeWindow judgeTimeWindow;

    /**
     * @brief The maximum angle diff accepted for ray casting.
     *
     * The note is judged as 'touched', if the casted touch ray has an angle smaller than specified
     * angle. This value is `1-cos(maxAngle)`, a small number near 0.
     */
    double judgeRayAngle = 0.015;

    /**
     * @brief Whether the PF judgement is enabled.
     *
     * Branch EXPERT can get PF grade. This is set to true only for EXPERT chars.
     */
    bool enablePFJudgement = false;

    /**
     * @brief Whether the alternatingg combo score should be used.
     *
     * Branch EXPERT has different combo value set.
     */
    bool enableAlternateCombo = false;

    /**
     * @brief The number of buff options provided during break.
     *
     * If a SUPPORT char exists in team, then its set to 4, otherwise 3.
     */
    unsigned int numBuffChoices = 3;

    /**
     * @brief Whether player can still hit notes when downed.
     *
     * Normally when a player is downed, they must wait until the next break and get
     * revived by teammate. Branch EXPERT can override this limit and be able to hit
     * notes with input latency before they're revived.
     */
    bool enableHitOnDowned = false;

    /**
     * @brief Whether the shield is enabled.
     */
    bool enableShield = true;

    /**
     * @brief Whether the detailed score is displayed.
     *
     * If not enabled, player won't be able to see the last two bits of their score.
     */
    bool enableDetailedScore = false;
};

#endif /* GAMEPLAY_RULES_GAMERULES */
