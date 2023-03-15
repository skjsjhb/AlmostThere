#ifndef GAMEPLAY_OBJS_TICKOBJECT
#define GAMEPLAY_OBJS_TICKOBJECT

#include "gameplay/anim/Animation.hh"
#include <list>

class TickObject
{
protected:
    std::list<Animation> animations;

public:
    virtual void tick(double absTime){};
    virtual ~TickObject() = default;
};

#endif /* GAMEPLAY_OBJS_TICKOBJECT */
