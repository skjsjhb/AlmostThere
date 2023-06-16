#ifndef ALMOSTTHERE_SRC_GAMEPLAY_PLAYER_PLAYEREVENTS_HH
#define ALMOSTTHERE_SRC_GAMEPLAY_PLAYER_PLAYEREVENTS_HH

#include "event/Event.hh"

enum class DamageType {
  Shield,
  Health,
};

class PlayerDamageEvent : public Event {
EVENT_HANDLERS(PlayerDamageEvent)
EVENT_DISPATCH
public:
  explicit PlayerDamageEvent(DamageType tp) : type(tp) {};
  DamageType getDamageType() { return type; }
protected:
  DamageType type;
};

class PlayerDieEvent : public Event {
EVENT_HANDLERS(PlayerDieEvent)
EVENT_DISPATCH
};

#endif // ALMOSTTHERE_SRC_GAMEPLAY_PLAYER_PLAYEREVENTS_HH
