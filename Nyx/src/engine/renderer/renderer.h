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
Sprite renderer_create_sprite(u32 texture_id, SDL_Rect rect, u32 sorting_layer, f32 sprite_size);
void renderer_draw_sprite(i32 x, i32 y, Sprite sprite);
void renderer_set_sprite_color(Sprite sprite, Texture_Color color);
void renderer_flip_sprite(Sprite sprite, SDL_RendererFlip flip_mode);
void renderer_set_sprite_size(Sprite sprite, f32 size);

// Shapes
void renderer_draw_line(i32 x1, i32 y1, i32 x2, i32 y2, Texture_Color color);
void renderer_draw_circle(i32 x, i32 y, u32 radius, Texture_Color color);

// UI
void renderer_draw_text(i32 x, i32 y, Text data);
void renderer_set_text_color(Text data, Texture_Color color);
void renderer_set_text_size(Text data, f32 size);

void renderer_draw_image(i32 x, i32 y, Image data);
void renderer_set_image_color(Image data, Texture_Color color);
void renderer_set_image_size(Image data, f32 size);
