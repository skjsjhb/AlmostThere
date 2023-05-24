/**
 * @brief Note object tick/render proxy module.
 *
 * This module is a wrapper of an obj controller and some misc state, with additional judgement
 * and render method implemented.
 *
 * @author skjsjhb
 */

#ifndef GAMEPLAY_OBJS_NOTE
#define GAMEPLAY_OBJS_NOTE

#include <cglm/cglm.h>
#include "TickObject.hh"
#include "gameplay/control/Controller.hh"
#include "Slot.hh"
#include "NoteDef.hh"
#include "engine/virtual/Graphics.hh"
#include "gameplay/input/InputSet.hh"
#include "gameplay/score/Score.hh"

enum JudgeStage
{
    BUSY,   // The area is not cleared, other inputs may be present (for previous notes)
    CLEAR,  // Zone cleared, but not yet able to judge (turn to busy if input detected)
    ACTIVE, // Now accepting input (any input detected will start the judge)
    JUDGED, // Judge finished, either missed or completed
};

class Game;

// Represents a loaded note
class Note : public TickObject
{
public:
    virtual void tick(double absTime) override;
    Note(NoteType tp, Game &g) : typ(tp), game(g){};
    static std::shared_ptr<Note> createNote(std::weak_ptr<NoteObject> obj, Game &g);

protected:
    /**
     * @brief Reserved judge method for compatibility with existing structure.
     * @param absTime Current time.
     * @param input Input buffer.
     * @param sm Score manager, for rules retreiving.
     */
    virtual void performJudge(double absTime, InputSet &input, ScoreManager &sm) = 0;
    NoteType typ;
    JudgeStage judgeStage = BUSY;
    // Fake notes are not judged but will be ticked
    bool isFake = false;
    bool isActive = true;

    // Reserved
    // NoteElementType element;

    // Game reference
    Game &game;
};

/**
 * @brief A singleton note interface which implements singleton note judging.
 *
 * This class implements snigleton note judge algorithm and can be directly inherited to use
 * the defined `performJudge` method.
 */
class SingletonNote : public Note
{
public:
    // Inherited constructor
    using Note::Note;

protected:
    /**
     * @brief Perform the judgement of the note.
     *
     * This function judges the note according to its state and input buffer. If a judge
     * completed, a score might be pushed into the score record set.
     *
     * @note This function is marked as non-virtual and therefore should not be overriden.
     * This is worked as intended.
     * @param absTime Current time.
     * @param input The input buffer set.
     * @param sm The score record container to push score in.
     */
    virtual void performJudge(double absTime, InputSet &input, ScoreManager &sm) override;
};

class Tapu : public SingletonNote
{
public:
    void draw(DrawContext &ctx) override;
    Tapu(Game &g) : SingletonNote(TAPU, g){};
};

class Shizuku : public Note
{
public:
    void draw(DrawContext &ctx) override;
    Shizuku(Game &g) : Note(SZKU, g){};

protected:
    void performJudge(double absTime, InputSet &input, ScoreManager &sm) override;
};

class Puresu : public Note
{
public:
    void draw(DrawContext &ctx) override;
    Puresu(Game &g) : Note(PRSU, g) {}

protected:
    void performJudge(double absTime, InputSet &input, ScoreManager &sm) override;
    double lastSuccJudge = -1; // Timestamp of the last successful judgement.
};

class Kyozetsu : public Note
{
public:
    void draw(DrawContext &ctx) override;
    Kyozetsu(Game &g) : Note(KZTU, g){};

protected:
    void performJudge(double absTime, InputSet &input, ScoreManager &sm) override;
};

class Hoshi : public SingletonNote
{
public:
    void draw(DrawContext &ctx) override;
    Hoshi(Game &g) : SingletonNote(HOSHI, g){};
};

class Hashi : public Note
{
public:
    void draw(DrawContext &ctx) override;
    Hashi(Game &g) : Note(HASHI, g){};

protected:
    void performJudge(double absTime, InputSet &input, ScoreManager &sm) override;
    double lastSuccJudge = -1;
    double judgedLength = 0;
};

#endif /* GAMEPLAY_OBJS_NOTE */
