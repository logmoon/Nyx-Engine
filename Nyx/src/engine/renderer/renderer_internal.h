# pragma once

# include <stdbool.h>
# include "../../../../include/SDL2/SDL.h"
# include "../types.h"
# include "../base_components/base_components.h"

typedef struct render_state
{
	SDL_Window* window;
	SDL_Renderer* renderer;

	u32 native_screen_width;
	u32 native_screen_height;

	u32 screen_width;
	u32 screen_height;

	int monitor_refresh_rate;

	bool fullscreen;

} Render_State;

typedef struct texutre_stack
{
	SDL_Texture** textures;
	u32 count;
	u32 cap;

}Texture_Stack;

# define INITIAL_TEXTURE_COUNT 10

bool renderer_init(char* window_name, u32 native_width, u32 native_height);
void renderer_shutdown();

bool renderer_textures_init(void);
u32 renderer_register_texture(SDL_Texture* texture);
void renderer_free_textures(void);

void renderer_render_sprites_system(void);
void renderer_render_text_system(void);
void renderer_render_images_system(void);
