# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "core_internal.h"
# include "core.h"
# include "../utils.h"
# include "../global.h"
# include "../renderer/renderer_internal.h"
# include "../renderer/camera_internal.h"
# include "../renderer/animator_internal.h"
# include "../audio/audio_internal.h"
# include "../io/io.h"
# include "../ecs/ecs.h"
# include "../scene_manager/scene_manager_internal.h"
# include "../window_events/window_events.h"
# include "../base_components/base_components.h"
# include "../physics/physics.h"
# include "../logger/logger.h"

# define FPS_CAP 200.0

void core_init(char* company_name,
		char* application_name,
		u32 native_width,
		u32 native_height,
		u32 window_width,
		u32 window_height,
		bool fullscreen)
{
	// Application
	Application_State app = { .company_name = company_name, .application_name = application_name };
	global.application_state = app;

	// Renderer
	if (!renderer_init(application_name, native_width, native_height, window_width, window_height, fullscreen))
	{
		ERROR_EXIT("\nCouldn't initialize core, error in the renderer module");
	}

	if (!audio_init())
	{
		ERROR_EXIT("\nCouldn't initialize core, errot in the audio module");
	}

	// IO
	if (!io_init(company_name, application_name))
	{
		ERROR_EXIT("\nCouldn't initialize core, error in the io module");
	}

	// ECS
	if (!ecs_init(7,
				sizeof(Position),
				sizeof(Sprite),
				sizeof(Animator),
				sizeof(Rigidbody), 
				sizeof(Circle_Collider),
				sizeof(Text),
				sizeof(Image)))
	{
		ERROR_EXIT("\n Couldn't initialize the ecs module");
	}

	// Scene Manager
	if (!scene_manager_init())
	{
		ERROR_EXIT("\nCouldn't initialize core, error in the scene manager module");
	}

	// Logger
	logger_init(LOG_LEVEL_DEBUG);
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
	f64 accumulator = 0.0;

	bool quit_flagged = false;

	while (!quit_flagged)
	{
		f64 new_time = SDL_GetTicks64() * 0.001;
		f64 frame_time = new_time - current_time;
		current_time = new_time;

		if (frame_time > 0.25)
		{
			frame_time = 0.25;
		}

		accumulator += frame_time;


		// Physics
		while (accumulator >= delta_time)
		{
			quit_flagged = !pump_events();
			if (quit_flagged) break;

			// Physics System
			const f32 physics_sub_dt = delta_time / PHYSICS_SUB_STEPS;
			for (u32 i = PHYSICS_SUB_STEPS; i--;)
			{
				physics_apply_gravity_system();
				physics_solve_collision_system();
				physics_update_positions_system(physics_sub_dt);
			}

			// Calling the active scene's update
			if (scene_manager_top(global.scene_manager)->update_func)
				scene_manager_top(global.scene_manager)->update_func(time, delta_time);

			// Update the camera
			camera_update();

			time += delta_time;
			accumulator -= delta_time;
		}


		// Rendering
		// Clearing the renderer
		SDL_RenderClear(global.renderer_state.renderer);
		// Animation
		animator_animate_system();
		// Sprite Component Rendering
		renderer_render_sprites_system();
		// Calling the active scene's Rendering function
		if (scene_manager_top(global.scene_manager)->draw_func)
			scene_manager_top(global.scene_manager)->draw_func();
		// UI Rendering
		// Rendering text
		renderer_render_text_system();
		// Rendering Images
		renderer_render_images_system();

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

	// Audio
	audio_shutdown();

	// Renderer
	renderer_shutdown(global.renderer_state.window, global.renderer_state.renderer);

	// SDL
	SDL_Quit();
}
