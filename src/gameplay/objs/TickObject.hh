#ifndef GAMEPLAY_OBJS_TICKOBJECT
#define GAMEPLAY_OBJS_TICKOBJECT

#include "gameplay/map/Controller.hh"
#include <memory>

#define OBJ_UNLOAD_DELAY 1

struct DrawContext;

class Game;

class TickObject {
public:
  /**
   * @brief Consider whether this object still needs to be ticked.
   * @return \c true if this object should be ticked.
   */
  [[nodiscard]] virtual bool shouldTick() const;

  /**
   * @brief Get the generate time (first tick time) of the object.
   *
   * The value returned are used for objects sorting. It's not required to return the accurate generate time (although
   * this impl does). In fact, even returing a proper pre-defined <tt>unsigned int</tt> indexes will work.
   *
   * \b However, the order have to be precise. i.e. if an object generates before another, then the former one \b MUST
   * return a value smaller than the latter one does.
   *
   * @return The generate time, or other index telling the order of the objects.
   */
  [[nodiscard]] virtual double getGenTime() const { return controller->getLifeTime().genTime; };

  /**
   * @brief Main tick method for any tickable object.
   *
   * Tickable objects will update their state on every tick. The speed of ticking is unstable and non-predictable, so
   * the use of timer is necessary to keep animation running at a preset speed.
   *
   * Call this method to update the state of the object.
   *
   * @note Passing \c absTime in a non-increasing order is considered a bad practice, as objects
   * might not always be able to handle reversed timeline correctly.
   */
  virtual void tick();

  virtual ~TickObject() = default;

  explicit TickObject(Game &g) : game(g) {};

  /**
   * @brief Object draw method.
   *
   * This method draw the object to the draw context of the bound game using the current status.
   * The context will be used by the engine to render it to the screen.
   */
  virtual void draw() {};

  /**
   * @brief Get the controller for necessary access.
   *
   * @note A controller should be fully managed by the owner in most cases. Modifying a controller
   * manually might cause potential desync.
   * @return A reference-pointer to the controller object.
   */
  std::weak_ptr<Controller> getController() {
    return controller;
  };

protected:
  std::shared_ptr<Controller> controller;

  // Game reference
  Game &game;
};

#endif /* GAMEPLAY_OBJS_TICKOBJECT */
