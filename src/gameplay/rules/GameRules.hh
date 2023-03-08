#ifndef GAMEPLAY_RULES_GAMERULES
#define GAMEPLAY_RULES_GAMERULES

struct JudgeValue
{
    unsigned int perfect, almost, accepted, medium, touched, lost; // Classified
    double spaceNotesAmplifier;                                    // Space notes weigh more
};

struct InputOptn
{
    double rayCastingAngleThreshold; // Allow
};

// Generated game rules
struct GameRules
{
    JudgeValue judgeValue;
};

#endif /* GAMEPLAY_RULES_GAMERULES */
