#ifndef ALMOSTTHERE_SRC_GAMEPLAY_OBJS_NOTEEVENTS_HH
#define ALMOSTTHERE_SRC_GAMEPLAY_OBJS_NOTEEVENTS_HH

#include "event/Event.hh"
#include "Note.hh"
#include "gameplay/score/ScoreValue.hh"

class Game;

class NoteEvent : public Event {
public:
  NoteEvent(Game &g, const Note &n) : game(g), note(n) {};

  Game &getGame() { return game; }

  const Note &getNote() { return note; }

protected:
  Game &game;
  const Note &note;
};

class NoteHitEvent : public NoteEvent {
EVENT_HANDLERS(NoteHitEvent)
EVENT_DISPATCH

public:
  NoteHitEvent(Game &g, const Note &n, ScoreGrade gd) : NoteEvent(g, n), grade(gd) {}

  [[nodiscard]] ScoreGrade getGrade() const { return grade; }

protected:
  ScoreGrade grade;
};

#endif // ALMOSTTHERE_SRC_GAMEPLAY_OBJS_NOTEEVENTS_HH
