#include "engine/virtual/Input.hh"

EVENT_HANDLERS_BODY(InputPressEvent)
EVENT_HANDLERS_BODY(InputReleaseEvent)
EVENT_HANDLERS_BODY(InputMoveEvent)
EVENT_HANDLERS_BODY(InputDragEvent)
EVENT_HANDLERS_BODY(InputClickEvent)

void vtInitInput() {}

std::list<const InputPoint *> vtGetInputPoints() { return {}; }