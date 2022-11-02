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
# include "sandbox/main_scene.h"

int main()
{
	// Initializing the logger
	logger_init(LOG_LEVEL_DEBUG);

	// Initializing the application
	core_init("UseCondomsKid",
			"Game",
			426,
			240);

	// Loading a font
	renderer_load_font("Assets/JetBrainsMono-Medium.ttf", 32);

	// Setting the entry scene
	Scene main = create_scene(&main_scene_init, &main_scene_update,  &main_scene_render, &main_scene_shutdown);
	set_active_scene(&main);

	// Update
	core_update();

	// Shutdown
	core_shutdown();

	return 0;
}
