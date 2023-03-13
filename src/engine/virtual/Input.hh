#ifndef ENGINE_VIRTUAL_INPUT
#define ENGINE_VIRTUAL_INPUT

#include "gameplay/input/InputSet.hh"

extern void vtPollEvents();

extern void vtSetActiveInputSet(InputSet *input);

extern void vtSetupKeyListener();

#endif /* ENGINE_VIRTUAL_INPUT */
