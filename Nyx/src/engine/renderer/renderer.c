# include "renderer.h"
# include "renderer_internal.h"
# include "../global.h"
# include "../utils.h"

// External


// Internal
bool renderer_init(char* window_name, u32 window_width, u32 window_height, bool fullscreen)
{
	SDL_Window *window = NULL;

	// Initializing SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		ERROR_RETURN(false, "Couldn't initialize SDL: %s", SDL_GetError());
	}
	printf("\nSuccessfully Initialized SDL");

	// Creating an SDL window
	u32 window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS;
	if (fullscreen) window_flags |= SDL_WINDOW_FULLSCREEN;

	window = SDL_CreateWindow(window_name,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			window_width,
			window_height,
			window_flags);

	if (window == NULL)
	{
        ERROR_RETURN(false, "\nCould not create window: %s", SDL_GetError());
    }
	printf("\nSuccessfully Created an SDL window");


	global.renderer_state.window = window;
	global.renderer_state.width = window_width;
	global.renderer_state.height = window_height;

	global.renderer_state.renderer = SDL_CreateRenderer(global.renderer_state.window,
			-1,
			SDL_RENDERER_ACCELERATED);

	if (global.renderer_state.renderer == NULL)
	{
		ERROR_RETURN(false, "\nCouldn't create the renderer: %s", SDL_GetError());
	}
	printf("\nSuccessfully Created the renderer");

	return true;
}

void renderer_shutdown(SDL_Window* window, SDL_Renderer* renderer) {
	SDL_DestroyWindow(window);
	window = NULL;

	SDL_DestroyRenderer(renderer);
	renderer = NULL;
}
