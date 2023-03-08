#ifndef GAMEPLAY_OBJS_TICKOBJECT
#define GAMEPLAY_OBJS_TICKOBJECT

enum TickStatus
{
    POSTPONE,   // Request a check later, use getNextTickTime() to get details
    LOAD_NOW,   // Should be loaded now
    TICK,       // Tick again on the next loop
    UNLOAD_NOW, // Can be safely unloaded now
    DISCARD,    // Discard this object
};

class TickObject
{
public:
    double getNextTickTime();
    TickStatus tick(double absTime);
};

#endif /* GAMEPLAY_OBJS_TICKOBJECT */
