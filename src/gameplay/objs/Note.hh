#ifndef GAMEPLAY_OBJS_NOTE
#define GAMEPLAY_OBJS_NOTE

#include <cglm/cglm.h>
#include "TickObject.hh"
#include "gameplay/control/Controller.hh"
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

class HitEffect : public TickObject
{
public:
    double startTime;
    bool isVisible = true;
    void tick(double absTime) override;
    void draw(DrawContext &ctx);
    HitEffect(vec3 pos, vec3 up, vec3 normal);

protected:
    vec3 pos, up, normal;
    double size = 0, initDirection, opacity = 1;
};

// Represents a loaded note
class AbstractNote : public TickObject
{
public:
    virtual void performJudge(double absTime, InputSet &input, ScoreManager &sm){}; // Perform judgement
    virtual void draw(DrawContext &ctx);                                            // Draw using current status
    void tick(double absTime) override;
    void bindSlot(Slot *slot);

    // Judgement helper
    JudgeStage jStage = BUSY;
    // Fake notes are not judged but will be ticked
    bool isFake = false;
    // Since the time it's loaded it will remain visible
    bool isVisible = true;
    NoteElementType element;
    // When will the note hit the slot
    // Used to control the position
    // This is the only position method that doesn't use animation
    double hitTime;
    vec3 basePosition, normal, up;
    // For assist-ring displaying
    Slot *targetSlot = nullptr;
    // Linked world
    World *world = nullptr;
    // Linked score manager
    // TODO: replace existing score manager with built-in
    ScoreManager *score = nullptr;

protected:
    // Hit effect
    bool playingHitEffect = false;
    double lastGenTime = 0; // Last hit effect played
    std::set<HitEffect *> hitEffects;
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

class Kyozetsu : public AbstractNote
{
public:
    void performJudge(double absTime, InputSet &input, ScoreManager &sm) override;
    void draw(DrawContext &ctx) override;
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
    double judgedLength;
};

#endif /* GAMEPLAY_OBJS_NOTE */
