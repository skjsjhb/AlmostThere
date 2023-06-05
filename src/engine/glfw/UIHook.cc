#include "engine/virtual/UIHook.hh"
#include "engine/virtual/Window.hh"
#include "util/Util.hh"

#include <unordered_map>
#include <utility>

static std::unordered_map<unsigned int, HookSt> uiHooksCtl;

static unsigned int chid = 1;

unsigned int vtAddUIHook(HookSt st) {
  uiHooksCtl[chid] = std::move(st);
  return chid++;
}

void vtDeUIHook(unsigned int hid) {
  uiHooksCtl.erase(hid);
}

bool isWithin(int v1, int v2, int v) {
  return (v1 <= v && v <= v2) || (v1 >= v && v >= v2);
}

// To prevent possible status desync, we use the same input buffer
void vtNotifyUIHooks(const InputBuffer &ibuf) {
  for (auto &hp : uiHooksCtl) {
    bool cli = true;
    for (auto &p : ibuf.touchPoints) {
      int sx, sy;
      vtDeCoord(int(p[0]), int(p[1]), sx, sy);
      // The Y-axis requires revert
      sy = 900 - sy;
      if (isWithin(hp.second.xbegin, hp.second.xend, sx) && isWithin(hp.second.ybegin, hp.second.yend, sy)) {
        if (hp.second.cleared) {
          hp.second.xt();
          hp.second.cleared = false;
        }
        cli = false;
      }
    }
    if (cli) {
      hp.second.cleared = true;
    }
  }
}
