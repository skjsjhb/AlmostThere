#ifndef GAMEPLAY_OBJS_NOTES_VARIANTS_RANKU
#define GAMEPLAY_OBJS_NOTES_VARIANTS_RANKU

#include "../FlatNote.hh"

class Ranku : public FlatNote
{
public:
    Ranku(Game &g) : Note(RKU, g), FlatNote(RKU, g)
    {
        sizew = 1.5;
        sizeh = 0.75;
        name = "ranku";
    };

    // We do not use `CatchNote` here, as we'll implement space judge
    void performJudge();
};

#endif /* GAMEPLAY_OBJS_NOTES_VARIANTS_RANKU */
