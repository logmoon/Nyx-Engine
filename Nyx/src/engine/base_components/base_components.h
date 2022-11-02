# pragma once
# include "../../../../include/SDL2/SDL.h"
# include "../types.h"

# define POSITION_COMPONENT 0
typedef struct position
{
	f32 x;
	f32 y;

}Position;

# define SPRITE_COMPONENT 1
typedef struct texture_color
{
	u8 r;
	u8 g;
	u8 b;
	u8 a;

}Texture_Color;
typedef struct sprite
{
	u32 texture_id;
	Rect rect;
	Texture_Color color;
	f64 rotation_angle;
	SDL_RendererFlip flip_mode;
	f32 size;
	u32 sorting_layer;

}Sprite;

# define ANIMATOR_COMPONENT 2
typedef enum animation_type
{
	NO_LOOP,
	FORWARD,
	PING_PONG

}Animation_Type;
typedef struct animation
{
	SDL_Rect first_frame;
	u32 frame_count;
	u32 frame_rate;
	Animation_Type animation_type;

}Animation;
typedef struct animator
{
	Animation* animations;
	u32 animation_cap;
	u32 animation_count;
	bool animation_changed;
	u32 current_animation;
	u32 frame_increase;
	u32 current_frame;
	u32 animation_timer;

}Animator;

// UI
# define TEXT_COMPONENT 3
typedef struct text
{
	u32 texture_id;
	Texture_Color color;
	f32 size;
	u32 width;
	u32 height;
	bool is_ui;
	u32 sorting_layer;

}Text;

# define IMAGE_COMPONENT 4
typedef struct image
{
	u32 texture_id;
	Texture_Color color;
	f32 size;
	u32 width;
	u32 height;
	bool is_ui;
	u32 sorting_layer;

}Image;


// Physics
# define RIGIDBODY_COMPONENT 5
# define CIRCLE_COLLIDER_COMPONENT 6
# define CONSTRAINT_COMPONENT 7
