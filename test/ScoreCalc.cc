#include "TestTools.hh"

#include "gameplay/core/Score.hh"
#include <cmath>

int main()
{
    GameRules gr;
    gr.judgeValue =
        {
            .perfect = 300,
            .almost = 200,
            .accepted = 150,
            .medium = 100,
            .touched = 50,
            .lost = 0,
            .spaceNotesAmplifier = 2,
        };
    ScoreManager sm(gr);
    sm.addJudgeGrade(PF, TAPU);
    sm.addJudgeGrade(PF, PRSU);
    WANT(sm.getAccuracyText() == "100.00%");
    WANT(sm.getCurrentScore() == 600);
    sm.addJudgeGrade(AC, HASHI);
    WANT(sm.getCurrentScore() == 900);
    WANT(sm.getFullScore() == 1200);
    WANT(sm.getAccuracyText() == "95.26%");
    sm.addJudgeGrade(AT, HOSHI);
    sm.addJudgeGrade(LT, SGKI);
    sm.addJudgeGrade(LT, SUI);
    WANT(sm.getCurrentScore() == 1300);
    sm.addJudgeGrade(TC, TSAR);
    WANT(sm.getCurrentScore() == 1400);
    TEND;
}