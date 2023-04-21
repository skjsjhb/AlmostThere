#include "TestTools.hh"

#include "engine/virtual/Audio.hh"
int main()
{
    vtInitAudio();
    WANT(vtPlaySound("a"));
    vtCloseAudio();
    TEND;
}