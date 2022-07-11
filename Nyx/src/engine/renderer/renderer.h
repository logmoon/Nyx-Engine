# pragma once
# include "../base_components/base_components.h"

SDL_Texture* renderer_load_texture(char* path);
Sprite renderer_create_sprite(SDL_Texture* tex, SDL_Rect rect);
void renderer_draw_sprite(int x, int y, Sprite sprite);
void renderer_set_sprite_color(Sprite sprite, Sprite_Color color);
void renderer_flip_sprite(Sprite sprite, SDL_RendererFlip flip_mode);
