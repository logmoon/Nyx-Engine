# pragma once

# include <stdbool.h>
# include "../../../../include/SDL2/SDL_mixer.h"
# include "../types.h"

# define MUSIC_INITIAL_CAP 3
# define SFX_INITIAL_CAP 10

typedef struct audio_stack
{
	Mix_Music** music;
	Mix_Chunk** sfx;
	u32 music_count;
	u32 music_cap;
	u32 sfx_count;
	u32 sfx_cap;

}Audio_Stack;

/**
 * Initializes the audio module
 *
 * @return returns true on success, false on failure
 */
bool audio_init(void);
/**
 * Loads and registers a sound effect
 *
 * @param path the path to the audio file
 * @return returns the id of the sound effect
 */
u32 audio_register_sfx(char* path);
/**
 * Loads and registers a music track
 *
 * @param path the path to the audio file
 * @return returns the id of the music track
 */
u32 audio_register_music(char* path);
/**
 * Shuts down the audio module
 *
 */
void audio_shutdown(void);
