#ifndef GAMEPLAY_OBJS_NOTES_VARIANTS_PURESU
#define GAMEPLAY_OBJS_NOTES_VARIANTS_PURESU

#include "../../Note.hh"

class Puresu : public Note
{
public:
    Puresu(Game &g) : Note(PRSU, g)
    {
        sizew = 1.0;
        sizeh = 1.0;
        name = "puresu";
    };

    void performJudge();
    void draw();

protected:
    double lastSuccJudge = -1; // Timestamp of the last successful judgement.
};

#endif /* GAMEPLAY_OBJS_NOTES_VARIANTS_PURESU */
