# pragma once

# include <stdbool.h>

# include "../../../../include/SDL2/SDL.h"
# include "../types.h"

typedef struct render_state
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	// ...
	u32 width;
	u32 height;

} Render_State;

bool renderer_init(char* window_name, u32 window_width, u32 window_height, bool fullscreen);
void renderer_shutdown(SDL_Window* window, SDL_Renderer* renderer);
