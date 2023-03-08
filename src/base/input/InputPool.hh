#ifndef BASE_INPUT_INPUTPOOL
#define BASE_INPUT_INPUTPOOL

#include <set>

enum InputType
{
    KDOWN, // Key just got pressed, can also be a mouse button
    KUP,   // Key just got released
    KHOLD, // Key is being held
    MMOVE, // Mouse is moving
    MPOS,  // Current mouse pos (should always be present if there is a mouse)
    MSCR,  // Mouse is scrolling
    TPOS,  // Touch pos
    TMOVE, // Touch pos move, requires os supporting
};

struct Input
{
    double time;
    bool used; // Already used to judge another note
    int btnValue;
    InputType type;
};

class InputPool
{

public:
protected:
    std::set<Input> inputList;
};

#endif /* BASE_INPUT_INPUTPOOL */
