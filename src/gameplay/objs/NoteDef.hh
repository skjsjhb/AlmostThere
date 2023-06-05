#ifndef GAMEPLAY_OBJS_NOTEDEF
#define GAMEPLAY_OBJS_NOTEDEF

enum NoteType {
  TAPU,  // Tapu
  PRSU,  // Puresu
  SZKU,  // Shizuku
  RKU,   // Ranku
  KZTU,  // Kyozetsu
  HOSHI, // Hoshi
  HASHI, // Hashi
  SUKI,  // Suki
};

#define NOTE_TYPE_SIZE 8

enum NoteElementType {
  FIRE,
  WIND,
  AQUA,
  SKY,
  NIGHT,
  GRASS,
  NONE
};

enum SlotVariant {
  CIRCLE,
  EUREKA,
  LINE,
};

#endif /* GAMEPLAY_OBJS_NOTEDEF */
