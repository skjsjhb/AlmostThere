#include "TestTools.hh"
#include "event/Event.hh"

class TestEvent : public Event {
EVENT_HANDLERS(TestEvent)
EVENT_DISPATCH

public:
  explicit TestEvent(int data) : dat(data) {};
  [[nodiscard]] int getData() const { return dat; }
protected:
  int dat;
};

class TestChildEvent : public TestEvent {
EVENT_HANDLERS(TestChildEvent)
EVENT_MIXED_DISPATCH(TestEvent)

public:
  explicit TestChildEvent(int data) : TestEvent(data) {};
  [[nodiscard]] int getDataMore() const { return dat + 1; };
};

EVENT_HANDLERS_BODY(TestEvent)
EVENT_HANDLERS_BODY(TestChildEvent)

int got = 0;

void testEventListener(TestEvent &e) {
  WANT(e.getData() == 1)
  got++;
}

void testChildEventListener(TestChildEvent &e) {
  WANT(e.getDataMore() == 2)
  got++;
}

int main() {
  addEventListener<TestEvent>(testEventListener);
  addEventListener<TestChildEvent>(testChildEventListener);

  TestChildEvent evt(1);
  evt.dispatch();
  WANT(got == 2)
  TEND
}