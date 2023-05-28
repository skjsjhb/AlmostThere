#include "ScoreRecord.hh"

unsigned int ScoreRecords::addRecord(std::unique_ptr<ScoreEntry> se)
{
    entries[eid] = std::move(se);
    return eid++;
}

unsigned int ScoreRecords::exportScore()
{
    amplifier = 1.0;
    total = 0;
    for (auto &a : entries)
    {
        a.second->apply(*this);
    }
    return total;
}

void ScoreRecords::rmRecord(unsigned int id)
{
    entries.erase(id);
}

void ScoreRecords::addAmplifier(double a)
{
    amplifier += a;
}

void ScoreRecords::addScore(unsigned int s)
{
    total += int(amplifier * s + 0.5);
}
