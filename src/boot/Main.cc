/**
 * @brief Temporal main module.
 *
 * This module does not have a header since it only acts as an entry point of the game.
 *
 * @author skjsjhb
 */

#include "Boot.hh"

int main() {
  sysInitFull();
  playDemoMap();
  sysStop();
  return 0;
}