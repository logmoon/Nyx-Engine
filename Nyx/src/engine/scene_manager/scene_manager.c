# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>

# include "scene_manager_internal.h"
# include "scene_manager.h"
# include "../ecs/ecs.h"
# include "../renderer/renderer_internal.h"
# include "../renderer/animator_internal.h"
# include "../utils.h"
# include "../global.h"

bool initialized = false;

// External
Scene create_scene(scene_init_func init, scene_update_func update, scene_draw_func draw, scene_shutdown_func shutdown)
{
	if (!initialized)
	{
		ERROR_EXIT("\nThe scene manager isn't initialized, can't create a scene");
	}

	Scene scene = { 0 };
	scene.init_func = init;
	scene.update_func = update;
	scene.draw_func = draw;
	scene.shutdown_func = shutdown;

	return scene;
}

void set_active_scene(Scene* scene)
{
	if (!initialized)
	{
		ERROR_EXIT("\nThe scene manager isn't initialized");
	}

	scene_manager_pop(global.scene_manager);
	scene_manager_push(global.scene_manager, scene);
}



// Internal
bool scene_manager_init()
{
	if (initialized)
	{
		ERROR_RETURN(false, "\nThe scene manager is already initialized");
	}

	global.scene_manager = malloc(sizeof(Scene_Manager));

	global.scene_manager->capacity = 3;
	global.scene_manager->stack = malloc(global.scene_manager->capacity * sizeof(Scene*));

	if (global.scene_manager->stack == NULL)
	{
		ERROR_RETURN(false, "\nCouldn't Initialize the scene manager, not enough memory");
	}

	global.scene_manager->top = -1;
	initialized = true;
	return true;
}

int scene_manager_shutdown(Scene_Manager* scene_manager)
{
	do
	{
		scene_manager_pop(scene_manager);

	} while (scene_manager->top > -1);


	free(scene_manager->stack);
	scene_manager->stack = NULL;
	free(global.scene_manager);
	global.scene_manager = NULL;
	initialized = false;
	return 0;
}

int scene_manager_scale(Scene_Manager* scene_manager)
{
	scene_manager->capacity *= 2;

	if ((scene_manager->stack = realloc(scene_manager->stack,
					scene_manager->capacity * sizeof(Scene*))) == NULL)
	{
		scene_manager_shutdown(scene_manager);
		ERROR_EXIT("\nCouldn't extend the scene manager stack, not enough memory");
	}

	return scene_manager->capacity;
}

int scene_manager_push(Scene_Manager* scene_manager, Scene* scene)
{
	if (scene_manager->top + 1 == scene_manager->capacity)
	{
		scene_manager_scale(scene_manager);
	}

	scene_manager->top++;
	scene_manager->stack[scene_manager->top] = scene;

	// ECS
	// Initializing the entites
	if (!ecs_scene_push())
	{
		ERROR_EXIT("\nCouldn't initialize the entities, when loading new scene");
	}

	// Renderer
	// Initializing the texutres
	if (!renderer_textures_init())
	{
		ERROR_EXIT("\nCouldn't initialize the textures, when loading new scene");
	}
	
	// Animator
	// Initializing the sprite animator
	if (!animator_init())
	{
		ERROR_EXIT("\nCouldn't initialize the sprite animator, when loading new scene");
	}

	if (scene->init_func) scene->init_func();

	return scene_manager->top;
}

int scene_manager_pop(Scene_Manager* scene_manager)
{
	if (scene_manager->top == -1) return 0;
	Scene* top = scene_manager_top(scene_manager);
	
	if (top->shutdown_func) top->shutdown_func();

	// ECS
	// Resetting the entites
	ecs_scene_pop();

	// Renderer
	// Freeing the texutres
	renderer_free_textures();

	// Animator
	// Freeing the animators and animations
	animator_free();

	scene_manager->stack[scene_manager->top] = NULL;
	scene_manager->top--;
	return scene_manager->top;
}

Scene* scene_manager_top(Scene_Manager* scene_manager)
{
	return scene_manager->stack[scene_manager->top];
}
