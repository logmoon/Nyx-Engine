# pragma once
# include "../base_components/base_components.h"
# include "../types.h"

SDL_Texture* renderer_get_texture(u32 texture_id);

// Loading
u32 renderer_load_sprite_texture(char* path);
bool renderer_load_font(char* font_path, u32 font_size);
Text renderer_load_text_texture(char* text, f32 text_size, bool is_ui);
Image renderer_load_image_texture(char* path, f32 image_size, bool is_ui);

// Sprites
Sprite renderer_create_sprite(u32 texture_id, SDL_Rect rect, f32 sprite_size);
void renderer_draw_sprite(int x, int y, Sprite sprite);
void renderer_set_sprite_color(Sprite sprite, Texture_Color color);
void renderer_flip_sprite(Sprite sprite, SDL_RendererFlip flip_mode);
void renderer_set_sprite_size(Sprite sprite, f32 size);

// UI
void renderer_draw_text(int x, int y, Text data);
void renderer_set_text_color(Text data, Texture_Color color);
void renderer_set_text_size(Text data, f32 size);

void renderer_draw_image(int x, int y, Image data);
void renderer_set_image_color(Image data, Texture_Color color);
void renderer_set_image_size(Image data, f32 size);
