#ifndef GAMEPLAY_OBJS_TICKOBJECT
#define GAMEPLAY_OBJS_TICKOBJECT

#include <list>

class TickObject
{
public:
    // Update self status
    virtual void tick(double absTime){};
    virtual ~TickObject() = default;
};

#endif /* GAMEPLAY_OBJS_TICKOBJECT */
