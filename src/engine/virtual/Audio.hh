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

/**
 * @brief Play an audio file from begin.
 *
 * @warning Start playing an audio already playing causes undefined behaviour.
 * @param sid The id of the sound object.
 */
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

/**
 * @brief Deinitialize audio library and destroy audio engine.
 */
extern void vtCloseAudio();

/**
 * @brief Set the speed of an audio.
 *
 * @note The pitch will change accordingly.
 * @param sid The id of the sound object.
 * @param spd Target speed.
 */
extern void vtAudioSpeed(unsigned int sid, float spd);

/**
 * @brief Resume the play of specified audio.
 *
 * @warning Resuming an audio already playing causes undefined behaviour.
 * @param sid The id of the sound object.
 */
extern void vtResumeAudio(unsigned int sid);

#endif /* ENGINE_VIRTUAL_AUDIO */
