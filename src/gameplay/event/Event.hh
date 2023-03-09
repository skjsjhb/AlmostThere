#ifndef GAMEPLAY_EVENT_EVENT
#define GAMEPLAY_EVENT_EVENT

#include <list>

enum EventType
{
    KEY,   // KBM button down or up
    POS,   // Mouse position change
    TOUCH, // Touch input status change
};

struct Event
{
    bool processed = false;
    double expectTime = 0; // Timer event or delay required
    union
    {
        unsigned int id;
        double value;
    };
};

class EventQueue
{
public:
    void pollEvents();
    std::list<Event> getCurrentEvents();

protected:
    std::list<Event> eventBuffer; // For scheduled events, sorted by time
    std::list<Event> currentEvents;
};

#endif /* GAMEPLAY_EVENT_EVENT */
