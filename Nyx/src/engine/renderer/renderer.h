# pragma once
# include "../base_components/base_components.h"
# include "../types.h"

u32 renderer_load_texture(char* path);
SDL_Texture* renderer_get_texture(u32 texture_id);
Sprite renderer_create_sprite(u32 texture_id, SDL_Rect rect);
void renderer_draw_sprite(int x, int y, Sprite sprite);
void renderer_set_sprite_color(Sprite sprite, Sprite_Color color);
void renderer_flip_sprite(Sprite sprite, SDL_RendererFlip flip_mode);
