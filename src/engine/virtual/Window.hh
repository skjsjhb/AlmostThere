#ifndef ENGINE_VIRTUAL_WINDOW
#define ENGINE_VIRTUAL_WINDOW

extern void vtInitWindow();

extern void *vtGetWindow();

extern void vtStopWindow();

// Return value specifies if this window should be closed
// True: Window should be closed now
extern bool vtWindowLoop();

extern void vtGetWindowSize(int &x, int &y);

#endif /* ENGINE_VIRTUAL_WINDOW */
