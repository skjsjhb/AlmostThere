#include "engine/virtual/Input.hh"

static InputBuffer ibuf;

void vtPollEvents() {}

void vtSetupListeners() {}

const InputBuffer &vtGetInputBuffer() { return ibuf; }
