# include "scene.h"
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include "../../engine/ecs/ecs.h"
# include "../../engine/global.h"
# include "../../engine/utils.h"
# include "../../engine/core/core.h"
# include "../../engine/audio/audio.h"
# include "../../engine/input/input.h"
# include "../../engine/renderer/renderer.h"
# include "../../engine/renderer/camera.h"
# include "../../engine/renderer/animator.h"
# include "../../engine/base_components/base_components.h"

typedef struct music
{
	u32 music;
	f32 bps;
	f32 start_delay;

	u32 bps_change_count;
	f32* bps_change_times;
	f32* bpss;
	u16 current_bps_change_index;

	u32 break_count;
	f32* break_durations;
	f32* break_times;
	u16 current_delay_index;

	u32 music_duration;
	u16 loops;

}Music;

typedef struct pool
{
	u32 pool_count;
	u32 active_positions;
	Entity* entities;

}Pool;


Music first = { .music = -1 };
f64 timer = 0;
f64 beat_timer = 0;
Pool arrow_pool;

int arrow_up_x = -48;
int arrow_down_x = -16;
int arrow_right_x = 16;
int arrow_left_x = 48;

int beat_arrow_start_y = 0;
int arrow_y = 0;
f32 arrow_y_error = 10;
int arrow_disappear_after = 32;

int start_score = 100;
int score_loss = 10;
int score_gain = 10;
int score = 0;

bool right;
bool left;
bool up;
bool down;

u32 arrows_texture = -1;

Sprite f_up;
Sprite f_down;
Sprite f_right;
Sprite f_left;


Pool pool_create_pool()
{
	Entity* entities = calloc(10, sizeof(Entity));

	if (entities == NULL)
	{
		ERROR_EXIT("Couldn't create pool, not enough memory");
	}


	for (u32 i = 0; i < 10; i++)
	{
		entities[i] = -1;
	}

	Pool pool = { .pool_count = 10, .active_positions = 0, .entities = entities };

	return pool;
}

Entity pool_get_entity(Pool pool, Position pos, Sprite sprite)
{
	for (u32 i = 0; i < pool.pool_count; i++)
	{
		if (pool.entities[i] == -1)
		{
			pool.entities[i] = ecs_create_entity();
			ecs_add_component(pool.entities[i], POSITION_COMPONENT, &pos);
			ecs_add_component(pool.entities[i], SPRITE_COMPONENT, &sprite);

			return pool.entities[i];
		}

		if (ecs_has_component(pool.entities[i], POSITION_COMPONENT))
		{
			Position* p = (Position*)ecs_get_component(pool.entities[i], POSITION_COMPONENT);

			if (p->x == 69 && p->y == 1000)
			{
				p->x = pos.x;
				p->y = pos.y;
	
				Sprite* spr = (Sprite*)ecs_get_component(pool.entities[i], SPRITE_COMPONENT);
				spr->texture_id = sprite.texture_id;
				spr->color = sprite.color;
				spr->flip_mode = sprite.flip_mode;
				spr->rect = sprite.rect;
				spr->rotation_angle = sprite.rotation_angle;
				spr->size = sprite.size;
				spr->sorting_layer = sprite.sorting_layer;
	
				return pool.entities[i];
			}
		}
	}

	pool.entities = realloc(pool.entities, pool.pool_count * 2 * sizeof(Entity));
	if (pool.entities == NULL)
	{
		ERROR_EXIT("Couldn't reallocate memory for pool, not enough memory");
	}
	for (u32 i = pool.pool_count - 1; i < pool.pool_count * 2; i++)
	{
		pool.entities[i] = -1;
	}

	pool.pool_count *= 2;

	return pool_get_entity(pool, pos, sprite);
}
void pool_reset_entity(Entity e)
{
	if (!ecs_has_component(e, POSITION_COMPONENT))
	{
		e = -1;
		return;
	}

	Position* p = (Position*)ecs_get_component(e, POSITION_COMPONENT);
	p->x = 69;
	p->y = 1000;
}


Beat_Arrow get_random_arrow(void)
{
	time_t t;
	srand((unsigned)time(&t));
	int ran = rand();

	srand(core_get_absolute_time());
	ran = (ran + rand()) % 4;

	return ran;
}

void display_arrow(Beat_Arrow arrow)
{
	int type = -1;
	switch(arrow)
	{
		case L:
			printf("left\n");
			Position l_pos = { .x = arrow_left_x, .y = beat_arrow_start_y };
			Entity l = pool_get_entity(arrow_pool, l_pos, f_left);
			if (!ecs_has_component(l, ARROW_COMPONENT))
			{
				type = 0;
				ecs_add_component(l, ARROW_COMPONENT, &type);
			}
			else
			{
				Arrow* a = (Arrow*)ecs_get_component(l, ARROW_COMPONENT);
				a->type = 0;
			}
			break;
		case R:
			printf("right\n");
			Position r_pos = { .x = arrow_right_x, .y = beat_arrow_start_y };
			Entity r = pool_get_entity(arrow_pool, r_pos, f_right);
			char r_char = 'r';
			if (!ecs_has_component(r, ARROW_COMPONENT))
			{
				type = 1;
				ecs_add_component(r, ARROW_COMPONENT, &type);
			}
			else
			{
				Arrow* a = (Arrow*)ecs_get_component(r, ARROW_COMPONENT);
				a->type = 1;
			}
			break;
		case U:
			printf("up\n");
			Position u_pos = { .x = arrow_up_x, .y = beat_arrow_start_y };
			Entity u = pool_get_entity(arrow_pool, u_pos, f_up);
			char u_char = 'u';
			if (!ecs_has_component(u, ARROW_COMPONENT))
			{
				type = 2;
				ecs_add_component(u, ARROW_COMPONENT, &type);
			}
			else
			{
				Arrow* a = (Arrow*)ecs_get_component(u, ARROW_COMPONENT);
				a->type = 2;
			}
			break;
		case D:
			printf("down\n");
			Position d_pos = { .x = arrow_down_x, .y = beat_arrow_start_y };
			Entity d = pool_get_entity(arrow_pool, d_pos, f_down);
			char d_char = 'd';
			if (!ecs_has_component(d, ARROW_COMPONENT))
			{
				type = 3;
				ecs_add_component(d, ARROW_COMPONENT, &type);
			}
			else
			{
				Arrow* a = (Arrow*)ecs_get_component(d, ARROW_COMPONENT);
				a->type = 3;
			}
			break;
	}
}

Music create_music(char* path, f32 bpm, u32 bpm_change_count, f32 start_delay, u32 break_count, u32 music_duration)
{
	Music music = { 0 };
	music.music = audio_load_music(path);
	music.bps = bpm / 60.0;
	music.start_delay = start_delay;
	music.break_count = break_count;
	music.music_duration = music_duration;
	music.current_delay_index = 0;
	music.current_bps_change_index = 0;
	music.loops = 1;

	music.bps_change_count = bpm_change_count;
	if ( bpm_change_count > 0)
	{
		music.bps_change_times = calloc(bpm_change_count, sizeof(f32));
		music.bpss = calloc(bpm_change_count, sizeof(f32));
	}

	if (break_count > 0)
	{
		music.break_durations = calloc(break_count, sizeof(f32));
		music.break_times = calloc(break_count, sizeof(f32));
	}

	if (music.bps_change_times == NULL ||
			music.bpss == NULL ||
			music.break_durations == NULL ||
			music.break_times == NULL)
	{
		ERROR_EXIT("Couldn't allocate memory for music track.");
	}

	return music;
}

void start_music(Music music, int fade_duration)
{
	audio_fade_in_music(first.music, -1, fade_duration);
	timer = 0;
	score = start_score;

	right = false;
	left = false;
	up = false;
	down = false;
}

void arrows_system(f32 bps, f32 delta_time)
{
	Ecs_Query_Result* qr = ecs_query(3, POSITION_COMPONENT, SPRITE_COMPONENT, ARROW_COMPONENT);

	for (u32 i = 0; i < qr->count; i++)
	{
		Position* p = (Position*)ecs_get_component(qr->entities[i], POSITION_COMPONENT);
		if (p->y != 1000)
		{
			if (p->y < arrow_y - arrow_disappear_after)
			{
				score -= score_loss;
				printf("Score%i\n", score);
				pool_reset_entity(qr->entities[i]);
			}
			else
			{
				p->y -= ((beat_arrow_start_y - arrow_y) / bps) * delta_time;
			}
		}
	}

	for (u32 i = 0; i < qr->count; i++)
	{
		Position* p = (Position*)ecs_get_component(qr->entities[i], POSITION_COMPONENT);
		if (p->y <= arrow_y + arrow_y_error && p->y >= arrow_y - arrow_y_error && p->x != 69)
		{
			Arrow* a = (Arrow*)ecs_get_component(qr->entities[i], ARROW_COMPONENT);
			if (key_is_down(SDL_SCANCODE_A) && !key_was_down(SDL_SCANCODE_A) && a->type == 0)
			{
				score += score_gain;
				printf("Score%i\n", score);
				pool_reset_entity(qr->entities[i]);
				return;
			}
			else if (key_is_down(SDL_SCANCODE_D) && !key_was_down(SDL_SCANCODE_D) && a->type == 1)
			{
				score += score_gain;
				printf("Score%i\n", score);
				pool_reset_entity(qr->entities[i]);
				return;
			}
			else if (key_is_down(SDL_SCANCODE_W) && !key_was_down(SDL_SCANCODE_W) && a->type == 2)
			{
				score += score_gain;
				printf("Score%i\n", score);
				pool_reset_entity(qr->entities[i]);
				return;
			}
			else if (key_is_down(SDL_SCANCODE_S) && !key_was_down(SDL_SCANCODE_S) && a->type == 3)
			{
				score += score_gain;
				printf("Score%i\n", score);
				pool_reset_entity(qr->entities[i]);
				return;
			}
		}
	}
}


void handle_music(Music first, f32 delta_time)
{
	timer += delta_time;

	// Timer for the start delay
	if (first.start_delay + (first.music_duration * (first.loops - 1)) - first.bps > timer)
	{
		return;
	}


	// systems
	arrows_system(first.bps, delta_time);

	// Timer for breaks
	if ((first.break_times[first.current_delay_index] - first.bps) * first.loops <= timer)
	{
		if ((first.break_times[first.current_delay_index] + first.break_durations[first.current_delay_index]
					- first.bps) * first.loops > timer)
		{
			return;
		}

		first.current_delay_index = (first.current_delay_index + 1) % first.break_count;
	}

	// Timer for bps changes
	if (first.bps_change_times[first.current_bps_change_index] * first.loops <= timer)
	{
		first.bps = first.bpss[first.current_bps_change_index];
		first.current_bps_change_index = (first.current_bps_change_index + 1) % first.bps_change_count;
	}

	if (beat_timer + (1/first.bps) <= timer)
	{
		display_arrow(get_random_arrow());
		beat_timer = timer;
	}


	if (first.music_duration * first.loops <= timer)
	{
		// song finished, it will loop
		first.loops++;
	}
}



void scene_init(void)
{
	global_set_level_dimentions(426, 240);
	arrows_texture = renderer_load_sprite_texture("Assets/Characters/arrows.png");
	beat_arrow_start_y = (global.renderer_state.native_screen_height / 2.0);
	
	Rect f_up_rect = { .x = 0, .y = 0, .w = 16, .h = 16 };
	Rect f_down_rect = { .x = 16, .y = 0, .w = 16, .h = 16 };
	Rect f_right_rect = { .x = 32, .y = 0, .w = 16, .h = 16 };
	Rect f_left_rect = { .x = 48, .y = 0, .w = 16, .h = 16 };

	Rect e_up_rect = { .x = 64, .y = 0, .w = 16, .h = 16 };
	Rect e_down_rect = { .x = 80, .y = 0, .w = 16, .h = 16 };
	Rect e_right_rect = { .x = 96, .y = 0, .w = 16, .h = 16 };
	Rect e_left_rect = { .x = 112, .y = 0, .w = 16, .h = 16 };

	f_up = renderer_create_sprite(arrows_texture, f_up_rect, 10, 1);
	f_down = renderer_create_sprite(arrows_texture, f_down_rect, 10, 1);
	f_right = renderer_create_sprite(arrows_texture, f_right_rect, 10, 1);
	f_left = renderer_create_sprite(arrows_texture, f_left_rect, 10, 1);

	Sprite e_up = renderer_create_sprite(arrows_texture, e_up_rect, 8, 1);
	Sprite e_down = renderer_create_sprite(arrows_texture, e_down_rect, 8, 1);
	Sprite e_right = renderer_create_sprite(arrows_texture, e_right_rect, 8, 1);
	Sprite e_left = renderer_create_sprite(arrows_texture, e_left_rect, 8, 1);

	Entity up = ecs_create_entity();
	Position up_pos = { .x = arrow_up_x, .y = arrow_y };
	ecs_add_component(up, POSITION_COMPONENT, &up_pos);
	ecs_add_component(up, SPRITE_COMPONENT, &e_up);

	Entity down = ecs_create_entity();
	Position down_pos = { .x = arrow_down_x, .y = arrow_y };
	ecs_add_component(down, POSITION_COMPONENT, &down_pos);
	ecs_add_component(down, SPRITE_COMPONENT, &e_down);

	Entity right = ecs_create_entity();
	Position right_pos = { .x = arrow_right_x, .y = arrow_y };
	ecs_add_component(right, POSITION_COMPONENT, &right_pos);
	ecs_add_component(right, SPRITE_COMPONENT, &e_right);

	Entity left = ecs_create_entity();
	Position left_pos = { .x = arrow_left_x, .y = arrow_y };
	ecs_add_component(left, POSITION_COMPONENT, &left_pos);
	ecs_add_component(left, SPRITE_COMPONENT, &e_left);

	arrow_pool = pool_create_pool();

	if (first.music == -1)
	{
		first = create_music("Assets/Audio/Music/AdhesiveWombat - Night Shade.mp3",
				140.8, 1, 2.0, 1, 411);

		first.break_times[0] = 230;
		first.break_durations[0] = 42;

		first.bps_change_times[0] = 272;
		first.bpss[0] = (280.0 / 60.0);
	}
	
	start_music(first, 5000);
}
void scene_update(f32 time, f32 delta_time)
{
	handle_music(first, delta_time);
}
void scene_render(void)
{
}
void scene_shutdown(void)
{
	// free the allocated shit
}
