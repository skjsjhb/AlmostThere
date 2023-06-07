#ifndef UI_LAYOUT_LAYOUTMANAGER
#define UI_LAYOUT_LAYOUTMANAGER

#include <list>

enum ComponentAlign {
  AL_BEGIN,
  AL_CENTER,
  AL_END,
};

struct ComponentCompileResult {
  bool ok = false;
  double x = 0, y = 0, w = 0, h = 0;
};

enum LengthMode {
  LM_PERCENT,
  LM_ABSOLUTE
};

class Length {
public:
  double value;
  LengthMode mode;

  Length() : value(0), mode(LM_ABSOLUTE) {};

  Length(double v) : value(v), mode(LM_ABSOLUTE) {}; // NOLINT Work as intended

  Length(double v, LengthMode m) : value(v), mode(m) {};
};

struct Component {
  Length dx, dy;
  Length w, h;
  ComponentAlign baseHorz = AL_BEGIN, baseVert = AL_BEGIN, selfHorz = AL_BEGIN, selfVert = AL_BEGIN;
  Component *parent = nullptr;
  ComponentCompileResult result;
};

class LayoutManager {
public:
  /**
   * @brief Add component into buffer for compilation.
   * @param c Component pointer to add.
   */
  void addComponent(Component *c);

  /**
   * @brief Compile all components and store their position info.
   */
  void compile();

protected:
  std::list<Component *> components;
};

#endif /* UI_LAYOUT_LAYOUTMANAGER */
