#include "engine/virtual/Audio.hh"

#include <map>
#include <string>
#include <miniaudio.h>

static ma_engine engine;

void vtInitAudio()
{
    ma_engine_init(NULL, &engine);
}

bool vtPlaySound(const std::string &wName)
{
    std::string fpt = "assets/audio/" + wName + ".wav";
    auto r = ma_engine_play_sound(&engine, fpt.c_str(), NULL);
    return r == MA_SUCCESS;
}

void vtCloseAudio()
{
    ma_engine_uninit(&engine);
}