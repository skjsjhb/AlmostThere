#ifndef ENGINE_VIRTUAL_AUDIO
#define ENGINE_VIRTUAL_AUDIO

#include <string>

extern void vtInitAudio();

/**
 * @brief Load an audio file and return its id.
 *
 * This function reads the content of the audio file and register it
 * as a single id.
 *
 * @see `vtUnloadAudio`
 * @param fname The relative path of the audio file.
 */
extern unsigned int vtLoadAudio(const std::string &fname);

/**
 * @brief Unload an audio file.
 *
 * @see `vtLoadAudio`
 * @param sid The id of the sound object.
 */
extern void vtUnloadAudio(unsigned int sid);

extern void vtPlayAudio(unsigned int sid);

/**
 * @brief Pause the audio object.
 *
 * @param sid The id of the sound object.
 */
extern void vtPauseAudio(unsigned int sid);

/**
 * @brief Reset audio buffer counter to 0, so the next call to `vtPlayAudio` will play from start.
 *
 * @warning Resetting audio progress counter during playing is undefined behaviour.
 * @param sid The id of the sound object.
 */
extern void vtResetAudio(unsigned int sid);

extern void vtCloseAudio();

#endif /* ENGINE_VIRTUAL_AUDIO */
