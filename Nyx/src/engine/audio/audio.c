# include <stdlib.h>
# include "../utils.h"

# include "audio_internal.h"
# include "audio.h"

Audio_Stack audio_stack;

// External
// Loading
u32 audio_load_sfx(char* path)
{
	return audio_register_sfx(path);
}
u32 audio_load_music(char* path)
{
	return audio_register_music(path);
}
// SFX
void audio_play_sfx(u32 sfx, u8 repeat_times)
{
	Mix_PlayChannel(-1, audio_stack.sfx[sfx], repeat_times);
}
void audio_set_sfx_volume(i16 volume_percent)
{
	Mix_Volume(-1, (MIX_MAX_VOLUME * volume_percent) / 100);
}
// Music
void audio_play_music(u32 music_track, i8 loops)
{
	Mix_PlayMusic(audio_stack.music[music_track], loops);
}
void audio_fade_in_music(u32 music_track, i8 loops, int fade_duration)
{
	Mix_FadeInMusic(audio_stack.music[music_track], loops, fade_duration);
}
void audio_fade_out_music(int fade_duration)
{
	Mix_FadeOutMusic(fade_duration);
}
void audio_set_music_volume(i16 volume_percent)
{
	Mix_VolumeMusic((MIX_MAX_VOLUME * volume_percent) / 100);
}
void audio_pause_music(void)
{
	Mix_PauseMusic();
}
void audio_resume_music(void)
{
	Mix_ResumeMusic();
}
void audio_stop_music(void)
{
	Mix_HaltMusic();
}
bool audio_music_paused(void)
{
	return Mix_PausedMusic() == 1;
}

// Internal
bool audio_init(void)
{
	//Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
		ERROR_RETURN(false, "Couldn't initialize SDL_mixer: %s\n", Mix_GetError());
    }

	// Initilizing the audio stack
	audio_stack.music = malloc(MUSIC_INITIAL_CAP * sizeof(Mix_Music*));
	audio_stack.sfx = malloc(SFX_INITIAL_CAP * sizeof(Mix_Chunk*));
	if (audio_stack.music == NULL || audio_stack.sfx == NULL)
	{
		printf("\nCouldn't allocate memory for audio");
		return false;
	}

	audio_stack.music_count = 0;
	audio_stack.sfx_count = 0;

	audio_stack.music_cap = MUSIC_INITIAL_CAP;
	audio_stack.sfx_cap = SFX_INITIAL_CAP;

	return true;
}

u32 audio_register_sfx(char* path)
{
	// Maximum sfxs reached, reallocating memory
	if (audio_stack.sfx_count == audio_stack.sfx_cap)
	{
		size_t new_size = audio_stack.sfx_cap * 2 * sizeof(Mix_Chunk*);
		audio_stack.sfx = realloc(audio_stack.sfx, new_size);
		if (audio_stack.sfx == NULL)
		{
			// Couldn't allocate memory
			printf("\nCouldn't reallocate memory for sound effects");
			return -1;
		}

		audio_stack.sfx_cap *= 2;
	}

	audio_stack.sfx[audio_stack.sfx_count] = Mix_LoadWAV(path);

	if (audio_stack.sfx[audio_stack.sfx_count] == NULL)
	{
		printf("\nFailed to load sound effect: %s\n", Mix_GetError());
		return -1;
	}

	audio_stack.sfx_count += 1;

	return audio_stack.sfx_count - 1;
}
u32 audio_register_music(char* path)
{
	// Maximum music tracks reached, reallocating memory
	if (audio_stack.music_count == audio_stack.music_cap)
	{
		size_t new_size = audio_stack.music_cap * 2 * sizeof(Mix_Music*);
		audio_stack.music = realloc(audio_stack.music, new_size);
		if (audio_stack.music == NULL)
		{
			// Couldn't allocate memory
			printf("\nCouldn't reallocate memory for music");
			return -1;
		}

		audio_stack.music_cap *= 2;
	}

	audio_stack.music[audio_stack.music_count] = Mix_LoadMUS(path);

	if (audio_stack.music[audio_stack.music_count] == NULL)
	{
		printf("\nFailed to load music: %s\n", Mix_GetError());
		return -1;
	}

	audio_stack.music_count += 1;

	return audio_stack.music_count - 1;
}

void audio_shutdown(void)
{
	// Freeing allocated music and sfx
	int i = 0;
	for (i = 0; i < audio_stack.music_count; i++)
	{
	 	Mix_FreeMusic(audio_stack.music[i]);
		audio_stack.music[i] = NULL;
	}
	for (i = 0; i < audio_stack.sfx_count; i++)
	{
		Mix_FreeChunk(audio_stack.sfx[i]);
		audio_stack.sfx[i] = NULL;
	}

	free(audio_stack.music);
	audio_stack.music = NULL;

	free(audio_stack.sfx);
	audio_stack.sfx = NULL;

	audio_stack.music_count = 0;
	audio_stack.music_cap = 0;
	audio_stack.sfx_count = 0;
	audio_stack.sfx_cap = 0;

	// Quit the SDL_mixer subsystem
	Mix_Quit();
}
