/**
 * @brief Major game boot module.
 *
 * This module initialize all game modules and acts as (partially) a bridge between os and engine.
 *
 * @author skjsjhb
 */

#ifndef BOOT_BOOT
#define BOOT_BOOT

struct Account;

/**
 * @brief Full init.
 *
 * Initializes all modules, including engine (graphics, audio, input, window, network), ui (render,
 * stat, input) and gameplay (objects, lua, map).
 */
void sysInitFull();

/**
 * @brief Start to play the demo map.
 *
 * Load and prepare the map, then start the game playing using a local account.
 */
void playDemoMap();

/**
 * @brief De-init system.
 *
 * Unload objects and stop system, then return for quitting.
 */
void sysStop();

#endif /* BOOT_BOOT */
