# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "core_internal.h"
# include "core.h"
# include "../utils.h"
# include "../global.h"
# include "../renderer/renderer_internal.h"
# include "../io/io.h"
# include "../ecs/ecs.h"
# include "../scene_manager/scene_manager_internal.h"
# include "../window_events/window_events.h"

# define FPS_CAP 200.0

typedef struct test_component
{
	f32 x;
	f32 y;

}Test_Component;

void core_init(char* company_name, char* application_name, u32 window_width, u32 window_height, bool fullscreen)
{
	// Application
	Application_State app = { .company_name = company_name, .application_name = application_name };
	global.application_state = app;

	// Renderer
	if (!renderer_init(application_name, window_width, window_height, fullscreen))
	{
		ERROR_EXIT("\nCouldn't initialize core, error in the renderer module");
	}

	// IO
	if (!io_init(company_name, application_name))
	{
		ERROR_EXIT("\nCouldn't initialize core, error in the io module");
	}

	// ECS
	if (!ecs_init(1, sizeof(Test_Component)))
	{
		ERROR_EXIT("\n Couldn't initialize the ecs module");
	}

	// Scene Manager
	if (!scene_manager_init())
	{
		ERROR_EXIT("\nCouldn't initialize core, error in the scene manager module");
	}
}

void core_update()
{
	// The Game Loop
	//
	// * Update
	//		* Physics
	//		* Input
	//		* ...
	//
	// * Draw on the screen
	
	// Initializing time variables
	f64 time = 0.0;
	f64 delta_time = 1 / FPS_CAP;

	f64 current_time = SDL_GetTicks64() * 0.001;
	// The accumulator represents how much time is required before another physics step can be taken.
	// The entire point of it is to decouple the physics and the rendering frame rates.
	f64 accumulator = 0.0;

	while (pump_events())
	{
		f64 new_time = SDL_GetTicks64() * 0.001;
		f64 frame_time = new_time - current_time;
		
		// If the frame is taking too long to run (more 1/4 seconds to be exact),
		// we set it to be 1/4 seconds
		if (frame_time > 0.25)
		{
			frame_time = 0.25;
		}

		current_time = new_time;
		accumulator += frame_time;

		// TODO Fix the timstep, it ain't right

		// Getting the currently active scene
		Scene* top = scene_manager_top(global.scene_manager);

		// Physics
		while (accumulator >= delta_time)
		{
			// Physics System
			// TODO

			time += delta_time;
			accumulator -= delta_time;
		}

		// Calling the active scene's update
		top->update_func(time, delta_time);

		// Rendering Systems
		// TODO
		// Just some place holder stuff
		SDL_RenderClear(global.renderer_state.renderer);
		SDL_RenderPresent(global.renderer_state.renderer);
	}
}

void core_shutdown(void)
{
	// Scene Manager
	scene_manager_shutdown(global.scene_manager);

	//ECS
	ecs_shutdown();

	// IO
	io_shutdown();

	// Renderer
	renderer_shutdown(global.renderer_state.window, global.renderer_state.renderer);
}
