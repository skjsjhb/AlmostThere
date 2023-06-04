#include "TestTools.hh"

#include "engine/virtual/Audio.hh"
#include <unistd.h>

int main()
{
    vtInitAudio();
    auto a = vtLoadAudio("test.flac");
    vtPlayAudio(a);
    sleep(2);
    vtPauseAudio(a);
    sleep(2);
    vtResumeAudio(a);
    sleep(2);
    vtCloseAudio();
    TEND;
}
