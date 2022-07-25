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
# include "engine/renderer/animator.h"
# include "engine/physics/physics.h"

Scene scene_one, scene_two;
Position* player_pos;
u32 music = -1;
u32 boom = -1;

/*
 *
 * -------- SCENE ONE --------
 *
 */
void scene_one_init(void)
{
	global_set_level_dimentions(1280, 720);

	//Loading audio, making sure that it isn't loaded more than once
	if (music == -1)
	{
		music = audio_load_music("Assets/BetterCallSaul.mp3");
	}
	if (boom == -1)
	{
		boom = audio_load_sfx("Assets/sfx.wav");
	}

	// u32 skeleton_tex = renderer_load_img_texture("Assets/skeleton.png");
	u32 saul_tex = renderer_load_sprite_texture("Assets/saul.png");

	// SDL_Rect idle_rect = { .x = 0, .y = 0, .w = 24, .h = 32 };
	// SDL_Rect attack_rect = { .x = 0, .y = 34, .w = 43, .h = 37 };
	SDL_Rect saul_rect = { .x = 0, .y = 0, .w = 118, .h = 90 };

	// Sprite player_sprite = renderer_create_sprite(skeleton_tex, idle_rect, 1.0);
	Sprite saul_sprite = renderer_create_sprite(saul_tex, saul_rect, 3.0);
	Position pos_temp = { 0 };

	// Create the animator
	Animator* animator = animator_create_animator();
	// Add animations
	// u32 idle = animator_add_animation(&animator, idle_rect, 11, 100, FORWARD);
	// u32 attack = animator_add_animation(&animator, attack_rect, 18, 100, NO_LOOP);
	u32 saul_anim = animator_add_animation(animator, saul_rect, 59, 100, FORWARD);
	animator_set_active_animation(animator, saul_anim);

	// Text
	Text other_text_data = renderer_load_text_texture("Other people: Create a game engine to make games", 0.5, true);
	Text me_text_data = renderer_load_text_texture("Me: Creates a game engine to code memes", 0.5, true);
	Text saul_text_data = renderer_load_text_texture("Saul Goodman:", 0.5, true);
	Text b_text_data = renderer_load_text_texture("Press B for funny sound effect", 0.4, true);
	Position other_pos = { .x = 0, .y = 210 };
	Position me_pos = { .x = 0, .y = 180 };
	Position saul_pos = { .x = 0, .y = 150 };
	Position b_pos = { .x = -300, .y = -220 };
	Entity other_text = ecs_create_entity();
	ecs_add_component(other_text, POSITION_COMPONENT, &other_pos);
	ecs_add_component(other_text, TEXT_COMPONENT, &other_text_data);
	Entity me_text = ecs_create_entity();
	ecs_add_component(me_text, POSITION_COMPONENT, &me_pos);
	ecs_add_component(me_text, TEXT_COMPONENT, &me_text_data);
	Entity saul_text = ecs_create_entity();
	ecs_add_component(saul_text, POSITION_COMPONENT, &saul_pos);
	ecs_add_component(saul_text, TEXT_COMPONENT, &saul_text_data);
	Entity b_text = ecs_create_entity();
	ecs_add_component(b_text, POSITION_COMPONENT, &b_pos);
	ecs_add_component(b_text, TEXT_COMPONENT, &b_text_data);

	// Physics
	/*
	Rigidbody rb = physics_create_rigidbody(1);
	Circle_Collider c = physics_create_circle_collider(1);
	*/

	Entity saul = ecs_create_entity();
	ecs_add_component(saul, POSITION_COMPONENT, &pos_temp);
	ecs_add_component(saul, SPRITE_COMPONENT, &saul_sprite);
	ecs_add_component(saul, ANIMATOR_COMPONENT, animator);
	// ecs_add_component(player, RIGIDBODY_COMPONENT, &rb);
	// ecs_add_component(player, CIRCLE_COLLIDER_COMPONENT, &c);
	
	player_pos = (Position*)ecs_get_component(saul, POSITION_COMPONENT);

	camera_init(NULL, 1.0);
}
void scene_one_update(f32 t, f32 dt)
{
	/*
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
	*/

	player_pos->x = get_mouse_world_position_x();
	player_pos->y = get_mouse_world_position_y();

	/*
	if (key_is_down(SDL_SCANCODE_O) && !key_was_down(SDL_SCANCODE_O))
	{
		Animator* animator = (Animator*)ecs_get_component(1, ANIMATOR_COMPONENT);
		animator_set_active_animation(animator, 0);
	}
	if (key_is_down(SDL_SCANCODE_P) && !key_was_down(SDL_SCANCODE_P))
	{
		Animator* animator = (Animator*)ecs_get_component(1, ANIMATOR_COMPONENT);
		animator_set_active_animation(animator, 1);
	}
	*/
	if (key_is_down(SDL_SCANCODE_B) && !key_was_down(SDL_SCANCODE_B))
	{
		audio_play_sfx(boom, 0);
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

	// Loading a font
	renderer_load_font("Assets/JetBrainsMono-Medium.ttf", 32);

	// Setting the entry scene
	set_active_scene(&scene_one);

	// Playing entry music
	audio_fade_in_music(music, -1, 5000);

	// Update
	core_update();

	// Shutdown
	core_shutdown();

	return 0;
}
