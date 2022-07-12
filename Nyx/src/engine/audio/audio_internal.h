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

bool audio_init(void);
u32 audio_register_sfx(char* path);
u32 audio_register_music(char* path);
void audio_shutdown(void);
