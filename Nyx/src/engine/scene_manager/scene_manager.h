# pragma once

# include "../types.h"

# ifndef SCENE_DATA
# define SCENE_DATA
typedef void(*scene_init_func)(void);
typedef void(*scene_update_func)(f32 time, f32 delta_time);
typedef void(*scene_shutdown_func)(void);

typedef struct scene
{
	scene_init_func		init_func;
	scene_update_func	update_func;
	scene_shutdown_func shutdown_func;

}Scene;
# endif

Scene create_scene(scene_init_func, scene_update_func, scene_shutdown_func);
void set_active_scene(Scene* scene);
