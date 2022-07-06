# pragma once
# include <stdbool.h>
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

typedef struct scene_manager
{
	Scene** stack;
	int capacity;
	int top;

}Scene_Manager;

bool scene_manager_init(Scene_Manager* scene_manager);
int scene_manager_shutdown(Scene_Manager* scene_manager);
int scene_manager_scale(Scene_Manager* scene_manager);
int scene_manager_push(Scene_Manager* scene_manager, Scene* scene);
int scene_manager_pop(Scene_Manager* scene_manager);
Scene* scene_manager_top(Scene_Manager* scene_manager);
