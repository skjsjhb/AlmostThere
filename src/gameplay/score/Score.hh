#ifndef GAMEPLAY_SCORE_SCORE
#define GAMEPLAY_SCORE_SCORE

#include "gameplay/rules/GameRules.hh"
#include "gameplay/objs/NoteDef.hh"
#include <map>
#include <string>

enum JudgeGrade
{
    PF,
    AT,
    AC,
    MD,
    TC,
    LT
};

class JudgeSummary
{
    friend class ScoreManager;

public:
    unsigned int getFullScore(const JudgeValue &va);
    unsigned int getTotalScore(const JudgeValue &va);

protected:
    std::map<JudgeGrade, unsigned int> notesCount;
    unsigned int getNoteCount();
    void addGrade(JudgeGrade grade);
};

class ScoreManager
{
    unsigned int combo, maxCombo;
    JudgeSummary nFlatJudges, nSpaceJudges;
    unsigned int bonus;

public:
    GameRules rules;

    ScoreManager();
    ScoreManager(const GameRules &rulesInit);
    void addBonusScore(unsigned int amount);
    double getAccuracy();
    std::string getAccuracyText();
    unsigned int getNoteCount();
    unsigned int getFullScore();
    unsigned int getCurrentScore();
    void addJudgeGrade(JudgeGrade grade, NoteType);
};

#endif /* GAMEPLAY_SCORE_SCORE */
