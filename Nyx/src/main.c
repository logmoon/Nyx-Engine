# include <stdio.h>
# include <stdbool.h>
# define SDL_MAIN_HANDLED
# include "engine/core/core.h"
# include "engine/input/input.h"
# include "engine/scene_manager/scene_manager.h"
# include "engine/ecs/ecs.h"
# include "engine/utils.h"
# include "engine/global.h"
# include "engine/base_components/base_components.h"
# include "engine/renderer/renderer.h"
# include "engine/renderer/camera.h"
# include "engine/audio/audio.h"
# include "engine/logger/logger.h"

Scene scene_one, scene_two;
Position* player_pos;

int music;
int sfx1;

/*
 *
 * -------- SCENE ONE --------
 *
 */
void scene_one_init(void)
{
	global_set_level_dimentions(1280, 720);

	//Audio
	music = audio_load_music("Assets/music.mp3");
	sfx1 = audio_load_sfx("Assets/sfx.wav");
	audio_play_music(music, -1);

	u32 player_tex = renderer_load_texture("Assets/Nyx_Dude.png");
	SDL_Rect player_rect = { .x = 0, .y = 0, .w = 32, .h = 32 };

	Sprite player_sprite = renderer_create_sprite(player_tex, player_rect);
	Position pos = { .x = 0, .y = 0 };

	Entity player = ecs_create_entity();
	ecs_add_component(player, POSITION_COMPONENT, &pos);
	ecs_add_component(player, SPRITE_COMPONENT, &player_sprite);

	player_pos = (Position*)ecs_get_component(0, POSITION_COMPONENT);

	logger_log_message(LOG_LEVEL_INFO, "HAHAHAHAHAHAHAHAHAHA, Here's the player's id:%u", player);

	camera_init(player_pos);
}
void scene_one_update(f32 t, f32 dt)
{
	if (key_is_down(SDL_SCANCODE_D))
	{
		player_pos->x += 120 * dt;
	}
	if (key_is_down(SDL_SCANCODE_A))
	{
		player_pos->x -= 120 * dt;
	}
	if (key_is_down(SDL_SCANCODE_W))
	{
		player_pos->y += 120 * dt;
	}
	if (key_is_down(SDL_SCANCODE_S))
	{
		player_pos->y -= 120 * dt;
	}

	if (key_is_down(SDL_SCANCODE_B) && !key_was_down(SDL_SCANCODE_B))
	{
		audio_play_sfx(sfx1, 0);
	}
	if (key_is_down(SDL_SCANCODE_N) && !key_was_down(SDL_SCANCODE_N))
	{
		audio_set_music_volume(0);
	}

	if (key_is_down(SDL_SCANCODE_L) && !key_was_down(SDL_SCANCODE_L))
	{
		set_active_scene(&scene_two);
	}
}
void scene_one_draw(void)
{
}
void scene_one_shutdown(void)
{
}
/*
 *
 * -------- SCENE TWO -------- 
 */
void scene_two_init(void)
{
}
void scene_two_update(f32 t, f32 dt)
{
	if (key_is_down(SDL_SCANCODE_L) && !key_was_down(SDL_SCANCODE_L))
	{
		set_active_scene(&scene_one);
	}
}
void scene_two_draw(void)
{
}
void scene_two_shutdown(void)
{
}


int main()
{
	// Initializing the application
	core_init("UseCondomsKid",
			"Nyx Game",
			854,
			480,
			1280,
			720,
			false);

	// Creating the scenes
	scene_one = create_scene(&scene_one_init,
			&scene_one_update,
			&scene_one_draw,
			&scene_one_shutdown);

	scene_two = create_scene(&scene_two_init,
			&scene_two_update,
			&scene_two_draw,
			&scene_two_shutdown);

	// Setting the entry scene
	set_active_scene(&scene_one);

	// Update
	core_update();

	// Shutdown
	core_shutdown();

	return 0;
}
