# include "core_internal.h"
# include "core.h"
# include "../utils.h"
# include "../global.h"
# include "../renderer/renderer_internal.h"
# include "../io/io.h"
# include "../scene_manager/scene_manager_internal.h"
# include "../window_events/window_events.h"

# define FPS_CAP 60.0

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

	// Scene Manager
	Scene_Manager* scene_manager;
	if (!scene_manager_init(scene_manager))
	{
		ERROR_EXIT("\nCouldn't initialize core, error in the scene manager module");
	}
	global.scene_manager = scene_manager;
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
	// The accumulator represents how much time is required before another physics+update step can be taken.
	// The entire point of it is to decouple the physics+update and the rendering frame rates.
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

		// Getting the currently active scene
		Scene* top = scene_manager_top(global.scene_manager);

		// Physics and update loop
		while (accumulator >= delta_time)
		{
			// Physics
			// TODO

			// Calling the active scene's update
			top->update_func(time, delta_time);
			
			time += delta_time;
			accumulator -= delta_time;
		}

		// Rendering
		// TODO
	}
}

void core_shutdown(void)
{
	// Scene Manager
	scene_manager_shutdown(global.scene_manager);

	// IO
	io_shutdown();

	// Renderer
	renderer_shutdown(global.renderer_state.window, global.renderer_state.renderer);
}
