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
# include "engine/renderer/animator.h"
# include "engine/physics/physics.h"
# include "engine/logger/logger.h"

Scene scene_one, scene_two;
u32 texture = -1;

void scene_one_init(void)
{
	global_set_level_dimentions(1280, 720);
	printf("Hello from scene one\n");
	camera_init(NULL);
}
void scene_one_update(f32 t, f32 dt)
{
	if (key_is_down(SDL_SCANCODE_L) && !key_was_down(SDL_SCANCODE_L))
	{
		set_active_scene(&scene_two);
	}
}


void scene_two_init(void)
{
	printf("Hello from scene two\n");
}
void scene_two_update(f32 t, f32 dt)
{
	if (key_is_down(SDL_SCANCODE_L) && !key_was_down(SDL_SCANCODE_L))
	{
		set_active_scene(&scene_one);
	}
}


int main()
{
	// Initializing the logger
	logger_init(LOG_LEVEL_DEBUG);

	// Initializing the application
	core_init("UseCondomsKid",
			"Nyx Game",
			1280,
			720,
			1280,
			720,
			false);

	// Creating the scenes
	scene_one = create_scene(&scene_one_init,
			&scene_one_update,
			NULL,
			NULL);

	scene_two = create_scene(&scene_two_init,
			&scene_two_update,
			NULL,
			NULL);

	// Loading a font
	renderer_load_font("Assets/JetBrainsMono-Medium.ttf", 32);

	// Setting the entry scene
	set_active_scene(&scene_one);

	// Update
	core_update();

	// Shutdown
	core_shutdown();

	return 0;
}
