/**
 * @brief Note object tick/render proxy module.
 *
 * This module is a wrapper of an obj controller and some misc state, with
 * additional judgement and render method implemented.
 *
 * @author skjsjhb
 */

#ifndef GAMEPLAY_OBJS_NOTE
#define GAMEPLAY_OBJS_NOTE

#include "NoteDef.hh"
#include "TickObject.hh"

enum JudgeStage
{
  BUSY,   // The area is not cleared, other inputs may be present (for previous
          // notes)
  CLEAR,  // Zone cleared, but not yet able to judge (turn to busy if input
          // detected)
  ACTIVE, // Now accepting input (any input detected will start the judge)
  JUDGED, // Judge finished, either missed or completed
};

class Game;
struct NoteObject;

// Represents a loaded note
class Note : public TickObject
{
public:
  virtual void tick() override;
  Note(NoteType tp, Game &g) : TickObject(g), typ(tp){};

  static std::shared_ptr<Note> createNote(std::weak_ptr<NoteObject> obj,
                                          Game &g);

protected:
  /**
   * @brief Reserved judge method for compatibility with existing structure.
   */
  virtual void performJudge() = 0;
  bool isPressed();

  NoteType typ;
  JudgeStage judgeStage = BUSY;
  // Fake notes are not judged but will be ticked
  bool isFake = false;
  bool isActive = true;

  double sizew = 0, sizeh = 0;
  std::string name;
  bool use3DJudge = false;

  // Reserved
  // NoteElementType element;
};

#endif /* GAMEPLAY_OBJS_NOTE */
