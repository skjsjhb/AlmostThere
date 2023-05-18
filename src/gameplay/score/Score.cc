#include "Score.hh"

#include <cmath>
#include <iomanip>
#include <sstream>

unsigned int JudgeSummary::getTotalScore(const JudgeValue &va)
{
    return (notesCount[PF] * va.perfect + notesCount[AT] * va.almost + notesCount[AC] * va.accepted + notesCount[MD] * va.medium + notesCount[TC] * va.touched + notesCount[LT] * va.lost);
}

unsigned int JudgeSummary::getFullScore(const JudgeValue &va)
{
    // AT is of full score, PF has extra bonus.
    return va.almost * getNoteCount();
}

unsigned int JudgeSummary::getNoteCount()
{
    return notesCount[PF] + notesCount[AT] + notesCount[AC] + notesCount[MD] + notesCount[TC] + notesCount[LT];
}

ScoreManager::ScoreManager(const GameRules &rulesInit)
{
    rules = rulesInit; // Copy value
}

// Placeholder only
ScoreManager::ScoreManager()
{
    rules = {0};
}
double ScoreManager::getAccuracy()
{
    auto ra = std::sqrt((static_cast<double>(getCurrentScore()) * 100) / getFullScore()) * 11;
    return std::min(ra, 100.0);
}

std::string ScoreManager::getAccuracyText()
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << getAccuracy();
    return ss.str() + '%';
}

unsigned int ScoreManager::getNoteCount()
{
    return nFlatJudges.getNoteCount() + nSpaceJudges.getNoteCount();
}

unsigned int ScoreManager::getCurrentScore()
{
    return nFlatJudges.getTotalScore(rules.judgeValue) +
           rules.judgeValue.spaceNotesAmplifier * nSpaceJudges.getTotalScore(rules.judgeValue) + bonus;
}

unsigned int ScoreManager::getFullScore()
{
    return nFlatJudges.getFullScore(rules.judgeValue) +
           rules.judgeValue.spaceNotesAmplifier * nSpaceJudges.getFullScore(rules.judgeValue) + bonus;
}

void ScoreManager::addBonusScore(unsigned int a)
{
    bonus += a;
}

void ScoreManager::addJudgeGrade(JudgeGrade gd, NoteType xt)
{
    if (xt == HOSHI || xt == HASHI || xt == SGKI || xt == SUI || xt == TSAR)
    {
        nSpaceJudges.addGrade(gd);
    }
    else
    {
        nFlatJudges.addGrade(gd);
    }
}

void JudgeSummary::addGrade(JudgeGrade grade)
{
    notesCount[grade]++;
}