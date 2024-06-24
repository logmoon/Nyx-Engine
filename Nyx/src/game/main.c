# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>
# define SDL_MAIN_HANDLED
# include "../engine/core/core.h"
# include "../engine/scene_manager/scene_manager.h"
# include "../engine/utils.h"
# include "../engine/global.h"
# include "../engine/renderer/renderer.h"
# include "../engine/logger/logger.h"


Scene scene_one;

/*
 *
 * -------- SCENE ONE --------
 *
 */
void scene_one_init(void)
{
	global_set_level_dimentions(1280, 720);
	camera_init(NULL, 1.0);
}
void scene_one_update(f32 t, f32 dt)
{

}
void scene_one_draw(void)
{
}
void scene_one_shutdown(void)
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
