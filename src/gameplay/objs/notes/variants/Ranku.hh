#ifndef GAMEPLAY_OBJS_NOTES_VARIANTS_RANKU
#define GAMEPLAY_OBJS_NOTES_VARIANTS_RANKU

#include "../FlatNote.hh"
#include "../CatchNote.hh"

class Ranku : public FlatNote, public CatchNote {
public:
  explicit Ranku(Game &g) : Note(NoteType::RKU, g), FlatNote(NoteType::RKU, g), CatchNote(NoteType::RKU, g) {
    sizew = 1.5;
    sizeh = 0.75;
    name = "ranku";
  };
};

#endif /* GAMEPLAY_OBJS_NOTES_VARIANTS_RANKU */
