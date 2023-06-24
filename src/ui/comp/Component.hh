#ifndef ALMOSTTHERE_SRC_UI_COMP_COMPONENT_HH
#define ALMOSTTHERE_SRC_UI_COMP_COMPONENT_HH

#include <memory>
#include <string>
#include <list>
#include <map>
#include "ui/layout/Layout.hh"

class DrawList;
struct DrawContext;

typedef std::map<std::string, std::string> ComponentProps;

namespace Props {
typedef const std::string V;

V Width = "width";
V Height = "height";
V X = "x";
V Y = "y";
V Background = "background";
V Visible = "visible";
V Text = "text";
V Size = "size";
V Color = "color";
V XAlign = "xalign";
V YAlign = "yalign";
V Font = "font";
V External = "external";
};

class Component {

public:
  /**
   * @brief Draw this component to the target draw list.
   * @param d Target draw list.
   */
  virtual void draw(DrawList &d) = 0;

  // This is a dummy constructor, for the convinence of the initializing of derived class.
  // Note that the type of component creation is handled by the loader, not this module.
  Component(ComponentProps) {}; // NOLINT

  virtual ~Component() = default;

  /**
   * @brief Compute the layout of this component and its child.
   *
   * @warning Only call this method on the root component! Calling this method on a non-absolute positioning components
   * leads to undefined behaviour.
   */
  void computeLayout();

  /**
   * @brief Patch necessary info to the component.
   *
   * @note The list of child components are required to be moved. Since the children components are designed to be
   * managed by their parent only after being bound, it causes undefined behaviour to have a copy of them \b EVEN IF
   * they've already been copied before moving.
   *
   * @param[in,out] p Target component.
   * @param id The id of the component.
   * @param ch Child components.
   */
  static void patchComponent(const std::shared_ptr<Component> &p,
                             const std::string &id,
                             std::list<std::shared_ptr<Component>> &&ch);

protected:
  /**
   * @brief The id of a component.
   *
   * IDs are NOT strictly required to be different, except when indexing component using IDs.
   */
  std::string id;

  /**
   * @brief A reference-pointer to the parent component.
   */
  std::weak_ptr<Component> parent;

  /**
   * @brief An array of pointers to child components.
   *
   * @note The existence of a components relies on its parent. That is, if the parent component
   * has been destroyed, the child component will be automatically destroyed and removed.
   */
  std::list<std::shared_ptr<Component>> children;

  /**
   * @brief The layout unit to be used for layout computing.
   */
  LayoutUnit layout;
};

DrawContext &getDefaultDrawContext();

Align inferAlignMethod(const std::string &al);

void applyLayoutParams(ComponentProps props, LayoutUnit &layout);

#endif // ALMOSTTHERE_SRC_UI_COMP_COMPONENT_HH
