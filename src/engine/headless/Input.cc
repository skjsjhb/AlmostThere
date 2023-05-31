#include "engine/virtual/Input.hh"

static InputBuffer ibuf;

void vtSetupListeners() {}

const InputBuffer &vtGetInputBuffer() { return ibuf; }
