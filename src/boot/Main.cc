/**
 * @brief Temporal main module.
 *
 * This module does not have a header since it only acts as an entry point of the game.
 *
 * @author skjsjhb
 */

#include "Boot.hh"
#include <csignal>
#include <unistd.h>
#include <spdlog/spdlog.h>

void interruptCallback(int) {
  spdlog::warn("Interrupt signal received. VMC will try to terminate immediately. This is considered dangerous!");
  exit(0);
}

int main() {
  signal(SIGINT, interruptCallback); // Exit immediately
  sysInitFull();
  playDemoMap();
  sysStop();
  return 0;
}