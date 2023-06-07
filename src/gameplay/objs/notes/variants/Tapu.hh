#ifndef GAMEPLAY_OBJS_NOTES_VARIANTS_TAPU
#define GAMEPLAY_OBJS_NOTES_VARIANTS_TAPU

#include "../SingletonNote.hh"
#include "../FlatNote.hh"

class Tapu : public SingletonNote, public FlatNote {
public:
  explicit Tapu(Game &g) : Note(NoteType::TAPU, g), SingletonNote(NoteType::TAPU, g), FlatNote(NoteType::TAPU, g) {
    sizew = 1;
    sizeh = 1;
    name = "tapu";
  };
};

#endif /* GAMEPLAY_OBJS_NOTES_VARIANTS_TAPU */
