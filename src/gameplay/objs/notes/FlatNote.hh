#ifndef GAMEPLAY_OBJS_NOTES_FLATNOTE
#define GAMEPLAY_OBJS_NOTES_FLATNOTE

#include "../Note.hh"

/**
 * @brief A flat note interface which implements flat note drawing.
 *
 * This class implements flat note draw algorithm and can be directly inherited to use
 * the defined `draw` method.
 */
class FlatNote : virtual public Note {
public:
  FlatNote(NoteType t, Game &g) : Note(t, g) {}

protected:
  virtual void draw() override;
};

#endif /* GAMEPLAY_OBJS_NOTES_FLATNOTE */
