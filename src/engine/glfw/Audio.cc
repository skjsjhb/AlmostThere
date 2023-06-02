#include "engine/virtual/Audio.hh"

#include <map>
#include <string>
#include <miniaudio.h>
#include "spdlog/spdlog.h"
#include <memory>
#include "support/Resource.hh"

using namespace spdlog;

static ma_engine engine;

unsigned int iid = 1;

struct AudioObject
{
    ma_sound buffer; // Audio data buffer
};

std::map<unsigned int, std::unique_ptr<AudioObject>> audioObjectsIndex;

unsigned int
vtLoadAudio(const std::string &fname)
{
    auto aObj = std::make_unique<AudioObject>();
    if (ma_sound_init_from_file(&engine, fname.c_str(), MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, NULL, NULL, &aObj->buffer) != MA_SUCCESS)
    {
        warn("Could not load audio from: " + fname);
        return 0;
    }
    audioObjectsIndex[iid] = std::move(aObj);
    return iid++;
}

void vtInitAudio()
{
    info("Initializing audio library.");
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS)
    {
        error("Failed to initialize audio system. Is miniaudio working?");
    }
}

void vtUnloadAudio(unsigned int sid)
{
    if (audioObjectsIndex.contains(sid))
    {
        ma_sound_uninit(&audioObjectsIndex[sid]->buffer);
        audioObjectsIndex.erase(sid);
    }
}

void vtPlayAudio(unsigned int sid)
{
    if (audioObjectsIndex.contains(sid))
    {
        if (ma_sound_start(&audioObjectsIndex[sid]->buffer) != MA_SUCCESS)
        {
            warn("Could not play audio buffer: " + std::to_string(sid));
        }
    }
    else
    {
        warn("Playing non-existing audio buffer: " + std::to_string(sid));
    }
}

void vtCloseAudio()
{
    info("Cleaning audio buffers.");
    for (auto &p : audioObjectsIndex)
    {
        ma_sound_uninit(&p.second->buffer);
    }
    audioObjectsIndex.clear();
    info("Closing audio library.");
    ma_engine_uninit(&engine);
}

void vtPauseAudio(unsigned int sid)
{
    if (audioObjectsIndex.contains(sid))
    {
        ma_sound_stop(&audioObjectsIndex[sid]->buffer);
    }
    else
    {
        warn("Pausing non-existing audio buffer: " + std::to_string(sid));
    }
}

void vtResetAudio(unsigned int sid)
{
    if (audioObjectsIndex.contains(sid))
    {
        ma_sound_seek_to_pcm_frame(&audioObjectsIndex[sid]->buffer, 0);
    }
    else
    {
        warn("Resetting non-existing audio buffer: " + std::to_string(sid));
    }
}
