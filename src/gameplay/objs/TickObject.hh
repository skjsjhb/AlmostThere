#ifndef GAMEPLAY_OBJS_TICKOBJECT
#define GAMEPLAY_OBJS_TICKOBJECT

#include "gameplay/control/Controller.hh"

class TickObject
{
public:
    // Update self status
    virtual void tick(double absTime) = 0;
    virtual ~TickObject(){};
    ObjController *controller;
};

#endif /* GAMEPLAY_OBJS_TICKOBJECT */
