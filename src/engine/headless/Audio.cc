#include "engine/virtual/Audio.hh"

void vtInitAudio() {}

unsigned int vtLoadAudio(const std::string &) { return 1; }

void vtUnloadAudio(unsigned int) {}

void vtPlayAudio(unsigned int) {}

void vtPauseAudio(unsigned int) {}

void vtResetAudio(unsigned int) {}

void vtCloseAudio() {}

void vtAudioSpeed(unsigned int, float) {}

void vtResumeAudio(unsigned int) {}

void vtPlaySound(const std::string &s) {}