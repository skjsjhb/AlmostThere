#ifndef GAMEPLAY_OBJS_NOTE
#define GAMEPLAY_OBJS_NOTE

#include <cglm/cglm.h>
#include "TickObject.hh"
#include "Slot.hh"
#include "NoteDef.hh"
#include "gameplay/score/Score.hh"
#include "engine/virtual/Graphics.hh"
#include "gameplay/input/InputSet.hh"

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
    void tick(double absTime) override;

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
    void draw(DrawContext &ctx) override;
};

class Shizuku : public AbstractNote
{
public:
    void performJudge(double absTime, InputSet &input, ScoreManager &sm) override;
    void draw(DrawContext &ctx) override;
};

class Puresu : public AbstractNote
{
public:
    double length;    // Length (in seconds) of this puresu, will be adjusted during judgement
    double absLength; // Original length, read-only

    void performJudge(double absTime, InputSet &input, ScoreManager &sm) override;
    void draw(DrawContext &ctx) override;
    void tick(double absTime) override;

protected:
    double lastSuccJudge = -1; // Timestamp of the last successful judgement.
};

class Hoshi : public AbstractNote
{
public:
    void performJudge(double absTime, InputSet &input, ScoreManager &sm) override;
    void draw(DrawContext &ctx) override;
    void tick(double absTime) override;

protected:
    double assistRingScale;
};

class Hashi : public AbstractNote
{
public:
    double length;
    double absLength;
    void performJudge(double absTime, InputSet &input, ScoreManager &sm) override;
    void draw(DrawContext &ctx) override;
    void tick(double absTime) override;

protected:
    double assistRingScale;
    double lastSuccJudge = -1;
};
#endif /* GAMEPLAY_OBJS_NOTE */
