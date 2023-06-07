#ifndef GAMEPLAY_OBJS_NOTES_SHIZUKU
#define GAMEPLAY_OBJS_NOTES_SHIZUKU

#include "../FlatNote.hh"
#include "../CatchNote.hh"

class Shizuku : public FlatNote, public CatchNote {
public:
  explicit Shizuku(Game &g) : Note(NoteType::SZKU, g), FlatNote(NoteType::SZKU, g), CatchNote(NoteType::SZKU, g) {
    sizew = 0.6;
    sizeh = 0.6;
    name = "shizuku";
  };
};

#endif /* GAMEPLAY_OBJS_NOTES_SHIZUKU */
