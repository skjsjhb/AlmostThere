#ifndef GAMEPLAY_OBJS_NOTES_SINGLETONNOTE
#define GAMEPLAY_OBJS_NOTES_SINGLETONNOTE

#include "../Note.hh"

/**
 * @brief A singleton note interface which implements singleton note judging.
 *
 * This class implements snigleton note judge algorithm and can be directly inherited to use
 * the defined `performJudge` method.
 */
class SingletonNote : virtual public Note {
public:
  SingletonNote(NoteType t, Game &g) : Note(t, g) {}

protected:
  /**
   * @brief Perform the judgement of the note.
   *
   * This function judges the note according to its state and input buffer. If a judge
   * completed, a score might be pushed into the score record set.
   */
  void performJudge() override;
};

#endif /* GAMEPLAY_OBJS_NOTES_SINGLETONNOTE */
