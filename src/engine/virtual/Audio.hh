#ifndef ENGINE_VIRTUAL_AUDIO
#define ENGINE_VIRTUAL_AUDIO

#include <string>

extern void vtInitAudio();

extern bool vtPlaySound(const std::string &sname);

extern void vtCloseAudio();

#endif /* ENGINE_VIRTUAL_AUDIO */
