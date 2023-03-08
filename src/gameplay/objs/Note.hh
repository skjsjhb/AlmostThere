#ifndef GAMEPLAY_OBJS_NOTE
#define GAMEPLAY_OBJS_NOTE

#include "gameplay/judge/JudgeResponse.hh"

enum NoteType
{
    TAPU,  // Tapu
    PRSU,  // Puresu
    SZKU,  // Shizuku
    AKU,   // Aku
    HOSHI, // Hoshi
    HASHI, // Hashi
    SGKI,  // Sogeki
    SUI,   // Sui
    TSAR   // 4D Tesseract
};

enum NoteShape
{
    TRIANGLE,
    DIAMOND,
    HEXAGON,
    CIRCLE,
};

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

class AbstractNote
{
public:
    void shouldLoad(double absTime);
    void tick(double absTime);                  // Update position and status
    JudgeResponse performJudge(double absTime); // Perform judgement and get state
};

#endif /* GAMEPLAY_OBJS_NOTE */
