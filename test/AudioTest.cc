#include "TestTools.hh"

#include "engine/virtual/Audio.hh"
#include <unistd.h>

int main() {
  // This test is not done automatically. Manually check required.
  vtInitAudio();
  auto a = vtLoadAudio("test.flac");
  vtPlayAudio(a);
  sleep(1);
  vtPauseAudio(a);
  sleep(1);
  vtResumeAudio(a);
  sleep(1);
  vtCloseAudio();
  TEND
}
