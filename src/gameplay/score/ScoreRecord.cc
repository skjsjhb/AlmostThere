#include "ScoreRecord.hh"

unsigned int ScoreRecords::addRecord(const ScoreEntry &se)
{
    entries[eid] = se;
    return eid++;
}

unsigned int ScoreRecords::getSumScore()
{
    amplifier = 1.0;
    total = 0;
    for (auto &a : entries)
    {
        a.second.apply(*this);
    }
    return total;
}

void ScoreRecords::addAmplifier(double a)
{
    amplifier += a;
}

void ScoreRecords::addScore(unsigned int s)
{
    total += int(amplifier * s + 0.5);
}
