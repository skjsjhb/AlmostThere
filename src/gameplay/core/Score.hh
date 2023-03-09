#ifndef GAMEPLAY_CORE_SCORE
#define GAMEPLAY_CORE_SCORE

#include "gameplay/rules/GameRules.hh"
#include "gameplay/objs/Note.hh"

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
    unsigned int getNotesCount();
    void addGrade(JudgeGrade grade);
};

class ScoreManager
{
    GameRules rules;
    unsigned int combo, maxCombo;
    JudgeSummary nFlatJudges, nSpaceJudges;

public:
    ScoreManager(const GameRules &rulesInit);

    double getAccuracy();
    std::string getAccuracyText();
    unsigned int getFullScore();
    unsigned int getCurrentScore();
    void addJudgeGrade(JudgeGrade grade, NoteType);
};

#endif /* GAMEPLAY_CORE_SCORE */
