#ifndef GAMEPLAY_CONTROL_NOTECONTROL
#define GAMEPLAY_CONTROL_NOTECONTROL

#include <vector>
#include <cglm/cglm.h>
#include "gameplay/objs/TickObject.hh"

enum PositionMethod
{
    RELATIVE_GLOBAL,
    RELATIVE_LOCAL,
    ABSOLUTE,
};

class NoteController : TickObject
{
public:
    void tick(double absTime) override;

    // Get the three vectors.
    void getPos(vec3 posIn);
    void getUp(vec3 upIn);
    void getNorm(vec3 normIn);

protected:
};

#endif /* GAMEPLAY_CONTROL_NOTECONTROL */
