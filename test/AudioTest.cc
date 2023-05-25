#include "TestTools.hh"

#include "engine/virtual/Audio.hh"
#include <unistd.h>

int main()
{
    vtInitAudio();
    auto dea = vtLoadAudio("test.flac");
    WANT(dea != 0);
    vtUnloadAudio(dea);
    vtCloseAudio();
    TEND;
}