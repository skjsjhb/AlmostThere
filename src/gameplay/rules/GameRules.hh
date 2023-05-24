#ifndef GAMEPLAY_RULES_GAMERULES
#define GAMEPLAY_RULES_GAMERULES

struct JudgeValue
{
    unsigned int perfect, almost, accepted, medium, touched, lost; // Classified
    double spaceNotesAmplifier;                                    // Space notes weigh more
};

struct JudgeTime
{
    double range, good, almost, perfect, allowBreak;
};

struct InputOptn
{
    double judgeRayAngle; // For touch only
};

// Generated game rules
struct GameRules
{
    JudgeValue judgeValue;
    JudgeTime judgeTime;
    InputOptn inputOptn;

    /**
     * @brief The maximum angle diff accepted for ray casting.
     *
     * The note is judged as 'touched', if the casted touch ray has an angle smaller than specified
     * angle. This value is `1-cos(maxAngle)`, a small number near 0.
     */
    double judgeRayAngle;

    /**
     * @brief Whether the PF judgement is enabled.
     *
     * Branch EXPERT can get PF grade. This is set to true only for EXPERT chars.
     */
    bool enablePFJudgement;

    /**
     * @brief Whether the alternatingg combo score should be used.
     *
     * Branch EXPERT has different combo value set.
     */
    bool enableAlternateCombo;

    /**
     * @brief The number of buff options provided during break.
     *
     * If a SUPPORT char exists in team, then its set to 3, otherwise 2.
     */
    unsigned int numBuffChoices;

    /**
     * @brief Whether player can still hit notes when downed.
     *
     * Normally when a player is downed, they must wait until the next break and get
     * revived by teammate. Branch EXPERT can override this limit and be able to hit
     * notes with input latency before they're revived.
     */
    bool enableHitOnDowned;
};

#endif /* GAMEPLAY_RULES_GAMERULES */
