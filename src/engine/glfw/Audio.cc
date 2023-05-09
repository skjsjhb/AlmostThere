#include "engine/virtual/Audio.hh"

#include <map>
#include <string>
#include <miniaudio.h>
#include "spdlog/spdlog.h"
using namespace spdlog;

static ma_engine engine;

void vtInitAudio()
{
    info("Initializing audio library.");
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS)
    {
        error("Failed to initialize audio system. Is miniaudio working?");
    }
}

bool vtPlaySound(const std::string &wName)
{
    std::string fpt = "assets/audio/" + wName + ".wav";
    auto r = ma_engine_play_sound(&engine, fpt.c_str(), NULL);
    return r == MA_SUCCESS;
}

void vtCloseAudio()
{
    info("Closing audio library.");
    ma_engine_uninit(&engine);
}