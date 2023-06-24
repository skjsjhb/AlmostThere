#ifndef ALMOSTTHERE_SRC_GAMEPLAY_PLAYER_PLAYEREVENTS_HH
#define ALMOSTTHERE_SRC_GAMEPLAY_PLAYER_PLAYEREVENTS_HH

#include "event/Event.hh"

enum class DamageType {
  Shield,
  Health,
};

class Player;

class PlayerEvent : public Event {
public:
  explicit PlayerEvent(const Player &p) : player(p) {};

  const Player &getPlayer() { return player; }

protected:
  const Player &player;
};

class PlayerDamageEvent : public PlayerEvent {
EVENT_HANDLERS(PlayerDamageEvent)
EVENT_DISPATCH
public:
  explicit PlayerDamageEvent(const Player &p, DamageType tp) : PlayerEvent(p), type(tp) {};
  DamageType getDamageType() { return type; }

protected:
  DamageType type;
};

class PlayerDieEvent : public PlayerEvent {
EVENT_HANDLERS(PlayerDieEvent)
EVENT_DISPATCH
public:
  using PlayerEvent::PlayerEvent;
};

class PlayerShieldBreakEvent : public PlayerEvent {
EVENT_HANDLERS(PlayerShieldBreakEvent)
EVENT_DISPATCH
public:
  using PlayerEvent::PlayerEvent;
};

class PlayerCoreUnstableEvent : public PlayerEvent {
EVENT_HANDLERS(PlayerCoreUnstableEvent)
EVENT_DISPATCH
public:
  using PlayerEvent::PlayerEvent;
};

#endif // ALMOSTTHERE_SRC_GAMEPLAY_PLAYER_PLAYEREVENTS_HH
