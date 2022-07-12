# pragma once

# include <stdbool.h>

# include "../../../../include/SDL2/SDL_mixer.h"
# include "../types.h"

// Loading
/**
 * Loads a sound effect file
 *
 * @param path is the path of the file
 * @return the id of the registered sound effect, or -1 on failure
 */
u32 audio_load_sfx(char* path);

/**
 * Loads a music file
 *
 * @param path is the path of the file
 * @return the id of the registered music track, or -1 on failure
 */
u32 audio_load_music(char* path);

// SFX
/**
 * Plays a sound effect
 *
 * @param sfx is the id of the sound effect
 * @param repeat_times the number of times the sound effect should loop, -1 to loop infinitely (not really)
 */
void audio_play_sfx(u32 sfx, u8 repeat_times);

/**
 * Sets the volume of sound effects
 *
 * @param volume_percent volume percentage
 */
void audio_set_sfx_volume(i16 volume_percent);

// Music
// SFX
/**
 * Plays a music track
 *
 * @param music_track is the id of the music track
 * @param loops the number of times the music should loop, -1 to loop infinitely (not really)
 */
void audio_play_music(u32 music_track, i8 loops);

/**
 * Fades in a music track
 *
 * @param music_track is the id of the music track
 * @param loops the number of times the music should loop, -1 to loop infinitely (not really)
 * @param fade_duration is the fade in duration in seconds
 */
void audio_fade_in_music(u32 music_track, i8 loops, u8 fade_duration);

/**
 * Fades out and stops a music track
 *
 * @param fade_duration is the fade out duration in seconds
 */
void audio_fade_out_music(u8 fade_duration);

/**
 * Sets the volume of the music
 *
 * @param volume_percent volume percentage
 */
void audio_set_music_volume(i16 volume_percent);

/**
 * Pauses the music
 */
void audio_pause_music(void);

/**
 * Resumes the music
 */
void audio_resume_music(void);

/**
 * Stops the music
 */
void audio_stop_music(void);

/**
 * Checks if the music is paused
 *
 * @return returns true if the music is paused, false otherwise
 */
bool audio_music_paused(void);
