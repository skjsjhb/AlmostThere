#ifndef UI_LAYOUT_LAYOUTMANAGER
#define UI_LAYOUT_LAYOUTMANAGER

#include <list>
#include <string>

enum class Align {
  BEGIN,
  CENTER,
  END,
};

struct ComponentCompileResult {
  bool ok = false;
  double x = 0, y = 0, w = 0, h = 0;
};

enum class LengthMode {
  PERCENT,
  ABSOLUTE,
};

class Length {
public:
  double value;
  LengthMode mode;

  Length() : value(0), mode(LengthMode::ABSOLUTE) {};

  Length(double v) : value(v), mode(LengthMode::ABSOLUTE) {}; // NOLINT

  Length(double v, LengthMode m) : value(v), mode(m) {};

  // The string version constructor
  // Note that it does not run any check, the check of the value should be done statically.
  Length(std::string src); // NOLINT

  [[nodiscard]] double get(double parentValue) const;
};

struct LayoutUnit {
  Length dx = 0, dy = 0;
  Length w = 0, h = 0;
  Align baseHorz = Align::BEGIN, baseVert = Align::BEGIN, selfHorz = Align::BEGIN, selfVert = Align::BEGIN;
  LayoutUnit *parent = nullptr;
  ComponentCompileResult result;
};

/**
 * @brief Compute the layout of a unit.
 *
 * This method calculates parent layout first automatically if necessary.
 *
 * @param c A pointer to the layout to be computed.
 */
void computeLayoutUnit(LayoutUnit *c);

/**
 * @brief Get the default root unit. Usually this refers to the window root.
 * @return A pointer to the root unit.
 */
LayoutUnit *getRootUnit();

#endif /* UI_LAYOUT_LAYOUTMANAGER */
