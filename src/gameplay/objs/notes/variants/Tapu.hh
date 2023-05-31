#ifndef GAMEPLAY_OBJS_NOTES_VARIANTS_TAPU
#define GAMEPLAY_OBJS_NOTES_VARIANTS_TAPU

#include "../SingletonNote.hh"
#include "../FlatNote.hh"

class Tapu : public SingletonNote, public FlatNote
{
public:
    Tapu(Game &g) : Note(TAPU, g), SingletonNote(TAPU, g), FlatNote(TAPU, g)
    {
        sizew = 1;
        sizeh = 1;
        name = "tapu";
    };
};

#endif /* GAMEPLAY_OBJS_NOTES_VARIANTS_TAPU */
