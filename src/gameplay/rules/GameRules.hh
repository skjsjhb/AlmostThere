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
    double rayCastingAngleThreshold; // For touch only
};

// Generated game rules
struct GameRules
{
    JudgeValue judgeValue;
    JudgeTime judgeTime;
    InputOptn inputOptn;
};

#endif /* GAMEPLAY_RULES_GAMERULES */
