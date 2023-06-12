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

#define EVENT_HANDLERS(type) public: static HandlerList<type> &getHandlers() { return handlers; }; \
protected: static HandlerList<type> handlers;

#define EVENT_HANDLERS_BODY(type) HandlerList<type> type::handlers;

#define EVENT_DISPATCH public: virtual void dispatch() { dispatchEvent(*this); };

#define EVENT_MIXED_DISPATCH(base) public: virtual void dispatch() { dispatchEvent(*this); base::dispatch(); }

/**
 * @brief Base Event class.
 *
 * An event instance doesn't have to inherit this base class, as long as they implement static method \c getHandlers,
 * this event can be successfully dispatched and registered.
 *
 * However, for code simplicity and better understanding, we explicitly inherit from this class. Also, by using the
 * member function \c dispatch rather than the raw \c dispatchEvent, a child class can dispatch its parent event auto-
 * matically.
 *
 * @note Use macro \c EVENT_HANDLERS to define handlers for event dispatching.
 * @note Use macro \c EVENT_DISPATCH to define member function \c dispatch. Use \c EVENT_MIXED_DISPATCH to
 * dispatch both self and parent event.
 */
class Event {

public:
  virtual ~Event() = default;
  virtual void dispatch() = 0;
protected:

};

/**
 * @brief Dispatch an event.
 *
 * This method calls static method \c getHandlers of the event class to get all handlers, then
 * execute them in the order or registration.
 *
 * @note This method only dispatch the event itself. i.e. Listeners of its parent class won't get notified. To notify
 * parent listeners, use member function \c dispatch (if provided).
 *
 * @see \c Event
 * @tparam T Type of the event. Must provide static method <tt> static HandlerList<T>& getHandlers() </tt> to
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

template<typename T>
void removeEventListener(EventHandler<T> f) {
  auto &l = T::getHandlers();
  l.remove(f);
}

#endif // ALMOSTTHERE_SRC_GAMEPLAY_EVENT_EVENT_HH
