#include "engine/scene_manager/scene_manager_internal.h"
# include <stdio.h>
# include <stdbool.h>
# define SDL_MAIN_HANDLED
# include "engine/core/core.h"
# include "engine/input/input.h"
# include "engine/scene_manager/scene_manager.h"
# include "engine/ecs/ecs.h"
# include "engine/utils.h"
# include "engine/global.h"

typedef struct test_component
{
	f32 x;
	f32 y;

}Test_Component;

Scene scene_one, scene_two;

/*
 *
 *
 * -------- SCENE ONE --------
 *
 *
 */
void scene_one_init(void)
{
	// Create and initilize the entities
	// NOTE: Entities need to have a clear id so your can directly reference them in the other scene functions

	printf("\nInitializing scene one");
}
void scene_one_update(f32 t, f32 dt)
{
	// Go Crazy
	// Should be able to access entities of this scene
	// Call custom systems for custom components

	if (mouse_button_is_down(MOUSE_BUTTON_LEFT) && !mouse_button_was_down(MOUSE_BUTTON_LEFT))
	{
		for (int i = 0; i < 1000; i++)
		{
			printf("\n------------------------------------------------------");
			printf("\nCreating entity");
			Test_Component pos = { .x = 69, .y = 420 };
			Entity e = ecs_create_entity();
			if (e != -1)
			{
				printf("\nSuccessfully created entity: %d", e);
				if (ecs_add_component(e, 0, &pos))
				{
					printf("\nSuccessfully added component: %d", 0);
				}
			}
			else
			{
				printf("\nCouldn't create entity");
			}
		}
	}

	if (mouse_button_is_down(MOUSE_BUTTON_RIGHT) && !mouse_button_was_down(MOUSE_BUTTON_RIGHT))
	{
		Ecs_Query_Result* query = ecs_query(1, 0);

		printf("\nFound %u entities with component: %u", query->count, 0);
	}

	if (key_is_down(SDLK_a) && !key_was_down(SDLK_a))
	{
		set_active_scene(&scene_two);
	}
}
void scene_one_shutdown(void)
{
	// Deallocate any memory, and shutdown
	printf("\nShutting down scene one");
}



/*
 *
 *
 * -------- SCENE TWO --------
 *
 *
 */
void scene_two_init(void)
{
	// Create and initilize the entities
	// NOTE: Entities need to have a clear id so your can directly reference them in the other scene functions

	printf("\nInitializing scene two");
}
void scene_two_update(f32 t, f32 dt)
{
	// Go Crazy
	// Should be able to access entities of this scene
	// Call custom systems for custom components

	if (mouse_button_is_down(MOUSE_BUTTON_LEFT) && !mouse_button_was_down(MOUSE_BUTTON_LEFT))
	{
		for (int i = 0; i < 1000; i++)
		{
			printf("\n------------------------------------------------------");
			printf("\nCreating entity");
			Test_Component pos = { .x = 69, .y = 420 };
			Entity e = ecs_create_entity();
			if (e != -1)
			{
				printf("\nSuccessfully created entity: %d", e);
				if (ecs_add_component(e, 0, &pos))
				{
					printf("\nSuccessfully added component: %d", 0);
				}
			}
			else
			{
				printf("\nCouldn't create entity");
			}
		}
	}

	if (mouse_button_is_down(MOUSE_BUTTON_RIGHT) && !mouse_button_was_down(MOUSE_BUTTON_RIGHT))
	{
		Ecs_Query_Result* query = ecs_query(1, 0);

		printf("\nFound %u entities with component: %u", query->count, 0);
	}

	if (key_is_down(SDLK_a) && !key_was_down(SDLK_a))
	{
		set_active_scene(&scene_one);
	}
}
void scene_two_shutdown(void)
{
	// Deallocate any memory, and shutdown
	printf("\nShutting down scene two");
}



int main()
{
	// Initializing the application
	core_init("UseCondomsKid",
			"Nyx Game",
			1280,
			720,
			false);

	// Creating the scenes
	scene_one = create_scene(&scene_one_init, &scene_one_update, &scene_one_shutdown);
	scene_two = create_scene(&scene_two_init, &scene_two_update, &scene_two_shutdown);

	// Setting the entry scene
	set_active_scene(&scene_one);

	// Update
	core_update();

	// Shutdown
	core_shutdown();

	return 0;
}
