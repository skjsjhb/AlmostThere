#include "engine/virtual/Audio.hh"

void vtInitAudio() {}

unsigned int vtLoadAudio(const std::string &fname) { return 1; }

void vtUnloadAudio(unsigned int sid) {}

void vtPlayAudio(unsigned int sid) {}

void vtPauseAudio(unsigned int sid) {}

void vtResetAudio(unsigned int sid) {}

void vtCloseAudio() {}