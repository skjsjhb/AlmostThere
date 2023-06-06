#include "TestTools.hh"
#include "gameplay/event/Event.hh"

class TestEvent : public Event {
public:
  explicit TestEvent(int data) : dat(data) {};
  static HandlerList<TestEvent> &getHandlers() { return handlers; };
  static HandlerList<TestEvent> handlers;
  [[nodiscard]] int getData() const { return dat; }
protected:
  int dat;
};

HandlerList<TestEvent> TestEvent::handlers;

bool got = false;

void testEventListener(TestEvent &e) {
  WANT(e.getData() == 1)
  got = true;
}

void gbEventListener(Event &) {
  got = false;
}

int main() {
  addEventListener<TestEvent>(testEventListener);
  addEventListener<Event>(gbEventListener);

  TestEvent evt(1);
  dispatchEvent(evt);
  WANT(got)
  TEND
}