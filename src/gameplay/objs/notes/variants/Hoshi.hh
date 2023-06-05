#ifndef GAMEPLAY_OBJS_NOTES_VARIANTS_HOSHI
#define GAMEPLAY_OBJS_NOTES_VARIANTS_HOSHI

#include "../SingletonNote.hh"

class Hoshi : public SingletonNote {
public:
    void draw() override;

    explicit Hoshi(Game &g) : Note(HOSHI, g), SingletonNote(HOSHI, g) {
        sizew = 0.8;
        sizeh = 0.8;
        name = "hoshi";
        use3DJudge = true;
    }
};

#endif /* GAMEPLAY_OBJS_NOTES_VARIANTS_HOSHI */
