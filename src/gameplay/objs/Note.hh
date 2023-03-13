#ifndef GAMEPLAY_OBJS_NOTE
#define GAMEPLAY_OBJS_NOTE

#include <cglm/cglm.h>
#include "TickObject.hh"
#include "Slot.hh"
#include "gameplay/core/Score.hh"

enum NoteElementType
{
    FIRE,
    WIND,
    AQUA,
    SKY,
    NIGHT,
    GRASS,
    NONE
};

// Represents a loaded note
class AbstractNote : public TickObject
{
public:
    virtual void performJudge(double absTime, ScoreManager &sm); // Perform judgement
    virtual void draw();                                         // Draw using current status

protected:
    // The slot is used to determin the position of the note
    Slot targetSlot;
    // Fake notes are not judged but will be ticked
    bool isFake;
    // Auto: System will calculate the position based on fall speed and time
    // Manual: Block auto control and control it by animation only
    bool autoControl;
    NoteElementType element;
    // When will the note hit the slot
    // Used to control the position
    // This is the only position method that doesn't use animation
    double hitTime;
    vec3 basePosition;
    vec3 up;
};

class Tapu : public AbstractNote
{
public:
    void performJudge(double absTime, ScoreManager &sm);
    void tick(double absTime);
    void draw();
};

#endif /* GAMEPLAY_OBJS_NOTE */
