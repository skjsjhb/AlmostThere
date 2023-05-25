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

/**
 * @deprecated Use ScoreRecord instead.
 */
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
    unsigned int combo = 0, maxCombo = 0;
    JudgeSummary nFlatJudges, nSpaceJudges;
    unsigned int bonus = 0;

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
