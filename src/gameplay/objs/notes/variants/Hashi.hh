#ifndef GAMEPLAY_OBJS_NOTES_VARIANTS_HASHI
#define GAMEPLAY_OBJS_NOTES_VARIANTS_HASHI

#include "../../Note.hh"

class Hashi : public Note {
public:
  void performJudge() override;

  void draw() override;

  explicit Hashi(Game &g) : Note(NoteType::HASHI, g) {
    sizew = 0.6;
    sizeh = 0.6;
    name = "hashi";
    use3DJudge = true;
  };

protected:
  double lastSuccJudge = -1;
  double judgedLength = 0;
};

#endif /* GAMEPLAY_OBJS_NOTES_VARIANTS_HASHI */
