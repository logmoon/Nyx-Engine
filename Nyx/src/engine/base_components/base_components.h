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
typedef struct sprite_color
{
	u8 r;
	u8 g;
	u8 b;
	u8 a;

}Sprite_Color;

typedef struct sprite
{
	SDL_Texture* texture;
	SDL_Rect rect;
	Sprite_Color color;
	f64 rotation_angle;
	SDL_BlendMode blend_mode;
	SDL_RendererFlip flip_mode;

}Sprite;

# define TEST_COMPONENT_1 2
typedef struct test_component_1
{
	f32 a;
	f32 b;

}Test_Component_1;

# define TEST_COMPONENT_2 3
typedef struct test_component_2
{
	u8 yes;
	f32 no;
	Sprite_Color t;

}Test_Component_2;

