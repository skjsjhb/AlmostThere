#ifndef ENGINE_VIRTUAL_INPUT
#define ENGINE_VIRTUAL_INPUT

#include <glm/glm.hpp>
#include <set>
#include <list>

struct InputBuffer {
  std::list<glm::vec2> touchPoints;
  std::set<unsigned int> keys;
};

extern void vtSetupListeners();

extern const InputBuffer &vtGetInputBuffer();

#endif /* ENGINE_VIRTUAL_INPUT */
