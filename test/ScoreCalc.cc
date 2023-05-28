#include "TestTools.hh"

#include "gameplay/score/ScoreRecord.hh"

int main()
{
    ScoreRecords rd;
    rd.addRecord(NoteScoreEntry::create(TAPU, AT));
    WANT(rd.exportScore() == 12);
    rd.addAmplifier(1);
    rd.addRecord(NoteScoreEntry::create(TAPU, PF));
    WANT(rd.exportScore() == 54);
    TEND;
}
