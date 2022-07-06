# include <stdio.h>
# include <stdbool.h>
# define SDL_MAIN_HANDLED
# include "../../include/SDL2/SDL_timer.h"
# include "../../include/SDL2/SDL_image.h"
# include "engine/window_events/window_events.h"
# include "engine/input/input.h"
# include "engine/application/application.h"
# include "engine/renderer/renderer.h"
# include "engine/utils.h"
# include "engine/global.h"

# define FRAMES_PER_SECOND 60

int main()
{
	// Initialization
	// Creating the application
	Application_State app = application_create("UseCondomsKid", "Nyx Game");
	// Initializing the application
	application_init(app, 1280, 720, false);

	// Game Loop
	while(pump_events())
	{
		// Input
		// Keyboard keys test
		if (key_is_down(SDLK_b) && !key_was_down(SDLK_b))
		{
			printf("\nB is pressed");
		}
		// Mouse buttons test
		if (mouse_button_is_down(MOUSE_BUTTON_RIGHT) && !mouse_button_was_down(MOUSE_BUTTON_RIGHT))
		{
			printf("\nRight Click");
		}
		if (mouse_button_is_down(MOUSE_BUTTON_LEFT) && !mouse_button_was_down(MOUSE_BUTTON_LEFT))
		{
			printf("\nLeft Click");
		}
		if (mouse_button_is_down(MOUSE_BUTTON_MIDDLE) && !mouse_button_was_down(MOUSE_BUTTON_MIDDLE))
		{
			printf("\nMiddle Click");
		}

		// Physics

		// Rendering
		// Clearing the screen
		renderer_clear();

		// Sleeping
		SDL_Delay(1000/FRAMES_PER_SECOND);
	}

	// Shutdown
	application_shutdown();

	return 0;
}
