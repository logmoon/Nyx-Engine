# pragma once
# include "../../engine/types.h"

# define ARROW_COMPONENT 8
typedef enum beat_arrow
{
	L = 0,
	R = 1,
	U = 2,
	D = 3

}Beat_Arrow;

typedef struct arrow
{
	Beat_Arrow type;
}Arrow;

# define ENEMY_COMPONENT 9
typedef struct enemy
{
	char tmp;
}Enemy;

# define BG_COMPONENT 10
typedef struct bg
{
	char tmp;
}Bg;


void scene_init(void);
void scene_update(f32 time, f32 delta_time);
void scene_render(void);
void scene_shutdown(void);
