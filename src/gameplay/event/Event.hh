/**
 * @brief Almost There event system.
 * @author skjsjhb
 */

#ifndef ALMOSTTHERE_SRC_GAMEPLAY_EVENT_EVENT_HH
#define ALMOSTTHERE_SRC_GAMEPLAY_EVENT_EVENT_HH

#include <functional>
#include <list>

template<typename T>
using EventHandler = std::function<void(T &)>;

template<typename T>
using HandlerList = std::list<EventHandler<T>>;

/**
 * @brief Base Event class
 *
 * An event instance doesn't have to inherit this base class, as long as they implement static method `getHandlers()`,
 * this event can be successfully dispatched and registered. However, for code simplicity and better understanding, we
 * explicitly inherit from this class.
 */
class Event {
public:
  static HandlerList<Event> &getHandlers() { return handlers; };
  virtual ~Event() = default;
protected:
  static HandlerList<Event> handlers;
};

/**
 * @brief Dispatch an event.
 *
 * This method calls static method `getHandlers()` of the event class to get all handlers, then
 * execute them in the order or registration.
 *
 * @see `Event`
 * @tparam T Type of the event. Must provide static method `static HandlerList<T>& getHandlers()` to
 * get corresponding handlers.
 * @param e The event object.
 */
template<typename T>
void dispatchEvent(T &e) {
  HandlerList<T> handlers = T::getHandlers();
  for (auto &f : handlers) {
    f(e);
  }
}

template<typename T>
void addEventListener(EventHandler<T> f) {
  T::getHandlers().push_back(std::move(f));
}

#endif // ALMOSTTHERE_SRC_GAMEPLAY_EVENT_EVENT_HH
