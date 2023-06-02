#include "ScoreDisplay.hh"

#include "gameplay/base/Game.hh"

#define SCORE_VARY_TIME 0.3
#define SCORE_BITS 8
#define SCORE_XBEGIN 1350
#define SCORE_YBEGIN 820
#define SCORE_SIZE 0.8

void ScoreDisplay::draw()
{
    auto t = game.mapTimer.getTime();
    double alpha = 0.7;
    if (t - beginTime > SCORE_VARY_TIME)
    {
        displayScore = targetScore;
    }
    else
    {
        auto p = (t - beginTime) / double(SCORE_VARY_TIME);
        displayScore = p * targetScore + (1 - p) * beginScore;
        alpha = 1 - 0.3 * p;
    }

    auto s = std::to_wstring(displayScore);
    while (s.size() < SCORE_BITS)
    {
        s = L"0" + s;
    }
    if (!game.rules.enableDetailedScore)
    {
        s.replace(s.size() - 2, 2, L"**");
        s = s + L"?";
    }
    DrawParam p = {
        .ctx = game.ctxUI,
    };
    DisplayText text({SCORE_XBEGIN, SCORE_YBEGIN}, SCORE_SIZE, s, {1, 1, 1, alpha}, p);
    game.drawList.add(std::make_unique<DisplayText>(text));
}

void ScoreDisplay::setScore(unsigned int s)
{
    beginScore = targetScore;
    targetScore = s;
    beginTime = game.mapTimer.getTime();
    displayScore = beginScore;
}
