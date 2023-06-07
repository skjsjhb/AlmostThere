#ifndef GAMEPLAY_OBJS_NOTES_VARIANTS_KYOZETSU
#define GAMEPLAY_OBJS_NOTES_VARIANTS_KYOZETSU

#include "../FlatNote.hh"

class Kyozetsu : public FlatNote {
public:
  void performJudge() override;

  explicit Kyozetsu(Game &g) : Note(NoteType::KZTU, g), FlatNote(NoteType::KZTU, g) {
    sizew = 1;
    sizeh = 1;
    name = "kyozetsu";
  };
};

#endif /* GAMEPLAY_OBJS_NOTES_VARIANTS_KYOZETSU */
