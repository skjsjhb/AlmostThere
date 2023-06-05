#ifndef GAMEPLAY_OBJS_NOTES_CATCHNOTE
#define GAMEPLAY_OBJS_NOTES_CATCHNOTE

#include "../Note.hh"

class CatchNote : virtual public Note {
public:
  CatchNote(NoteType n, Game &g) : Note(n, g) {};

  void performJudge() override;
};

#endif /* GAMEPLAY_OBJS_NOTES_CATCHNOTE */
