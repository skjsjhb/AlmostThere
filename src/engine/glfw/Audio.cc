#include "engine/virtual/Audio.hh"

#include <map>
#include <string>
#include "engine/virtual/Framework.hh"
#include "spdlog/spdlog.h"
#include <memory>
#include <utility>
#include "soloud/soloud_wav.h"

using namespace spdlog;

SoLoud::Soloud engine;

unsigned int iid = 1;

struct AudioObject {
  unsigned int handler = 0;
  SoLoud::Wav sample;
};

std::map<unsigned int, std::unique_ptr<AudioObject>> audioObjectsIndex;
std::map<std::string, unsigned int> cachedSoundIndex;
std::map<std::string, double> throttleMap;

unsigned int
vtLoadAudio(const std::string &fname) {
  auto o = std::make_unique<AudioObject>();
  o->sample.load(fname.c_str());
  audioObjectsIndex[iid] = std::move(o);
  return iid++;
}

void vtInitAudio() {
  engine.init();
}

void vtUnloadAudio(unsigned int sid) {
  if (audioObjectsIndex.contains(sid)) {
    auto &pt = audioObjectsIndex[sid];
    pt->sample.stop();
    audioObjectsIndex.erase(sid);
  }
}

void vtPlayAudio(unsigned int sid) {
  if (audioObjectsIndex.contains(sid)) {
    auto &pt = audioObjectsIndex[sid];
    pt->handler = engine.play(pt->sample);
  }
}

void vtCloseAudio() {
  engine.stopAll();
  engine.deinit();
}

void vtPauseAudio(unsigned int sid) {
  if (audioObjectsIndex.contains(sid)) {
    auto &pt = audioObjectsIndex[sid];
    if (pt->handler != 0) {
      engine.setPause(pt->handler, true);
    }
  }
}

void vtResetAudio(unsigned int sid) {
}

void vtAudioSpeed(unsigned int sid, float spd) {
  if (audioObjectsIndex.contains(sid)) {
    auto &pt = audioObjectsIndex[sid];
    if (pt->handler != 0) {
      engine.setRelativePlaySpeed(pt->handler, spd);
    }
  }
}

void vtResumeAudio(unsigned int sid) {
  if (audioObjectsIndex.contains(sid)) {
    auto &pt = audioObjectsIndex[sid];
    if (pt->handler != 0) {
      engine.setPause(pt->handler, false);
    }
  }
}

#define THROTTLE_TIME 0.5

void vtPlaySound(const std::string &s, bool throttle) {
  if (!cachedSoundIndex.contains(s)) {
    auto id = vtLoadAudio(s);
    cachedSoundIndex[s] = id;
  }
  if (throttle) {
    if (!throttleMap.contains(s)) {
      throttleMap[s] = 0;
    }
    auto t = vtGetTime();
    if (t - throttleMap[s] <= THROTTLE_TIME) {
      return;
    }
    throttleMap[s] = t;
  }
  vtPlayAudio(cachedSoundIndex[s]);
}