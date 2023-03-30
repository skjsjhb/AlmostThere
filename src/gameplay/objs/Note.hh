#ifndef GAMEPLAY_OBJS_NOTE
#define GAMEPLAY_OBJS_NOTE

#include <cglm/cglm.h>
#include "TickObject.hh"
#include "Slot.hh"
#include "gameplay/core/Score.hh"
#include "engine/virtual/Graphics.hh"
#include "gameplay/input/InputSet.hh"

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

enum JudgeStage
{
    BUSY,   // The area is not cleared, other inputs may be present (for previous notes)
    CLEAR,  // Zone cleared, but not yet able to judge (turn to busy if input detected)
    ACTIVE, // Now accepting input (any input detected will start the judge)
    JUDGED, // Judge finished, either missed or completed
};

// Represents a loaded note
class AbstractNote : public TickObject
{
public:
    virtual void performJudge(double absTime, InputSet &input, ScoreManager &sm){}; // Perform judgement
    virtual void draw(DrawContext &ctx){};                                          // Draw using current status

    // For PC
    int keyCode;
    // Judgement helper
    JudgeStage jStage = BUSY;
    // The slot is used to determin the position of the note
    Slot *targetSlot;
    // Fake notes are not judged but will be ticked
    bool isFake;
    // Auto: System will calculate the position based on fall speed and time
    // Manual: Block auto control and control it by animation only
    bool autoControl;
    // Since the time it's loaded it will remain visible
    bool isVisible;
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
    void performJudge(double absTime, InputSet &input, ScoreManager &sm) override;
    void tick(double absTime) override;
    void draw(DrawContext &ctx) override;
};

#endif /* GAMEPLAY_OBJS_NOTE */
