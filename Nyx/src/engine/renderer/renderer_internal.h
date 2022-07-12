# pragma once

# include <stdbool.h>
# include "../../../../include/SDL2/SDL.h"
# include "../types.h"

typedef struct render_state
{
	SDL_Window* window;
	SDL_Renderer* renderer;

	u32 native_screen_width;
	u32 native_screen_height;

	u32 screen_width;
	u32 screen_height;

} Render_State;

typedef struct texutre_stack
{
	SDL_Texture** textures;
	u32 count;
	u32 cap;

}Texture_Stack;

# define INITIAL_TEXTURE_COUNT 10

bool renderer_init(char* window_name, u32 native_width, u32 native_height, u32 window_width, u32 window_height, bool fullscreen);
void renderer_shutdown(SDL_Window* window, SDL_Renderer* renderer);

bool renderer_textures_init();
u32 renderer_register_texture(char* path);
void renderer_free_textures(void);

void renderer_render_sprite_system(void);
