#include "engine/virtual/Graphics.hh"

void vtInitGraphics() {}

void vtDeInitGraphics() {}

int vtGetGraphicsError() { return 0; }

void vtSetBufferSize(int, int) {}

void vtSetBackground(const std::string &img) {}

void vtDrawList(DrawList &buf) {}

void Rect::draw() const {}

void Triangle::draw() const {}

void TriangleStrip::draw() const {}

void DisplayText::draw() const {}

void vtGetCharSize(const std::string &, wchar_t, float &, float &) {}