# include "renderer_internal.h"
# include "renderer.h"
# include "camera_internal.h"
# include "camera.h"
# include "../ecs/ecs.h"
# include "../base_components/base_components.h"
# include "../global.h"
# include "../utils.h"
# include "../../../../include/SDL2/SDL_image.h"


Texture_Stack texture_stack;

// Camera
void camera_init(Position* target)
{
	global.camera.camera_rect.x = (global.renderer_state.screen_width - global.renderer_state.native_screen_width) / 2.0;
	global.camera.camera_rect.y = (global.renderer_state.screen_height - global.renderer_state.native_screen_height) / 2.0;
	
	global.camera.camera_rect.w = global.renderer_state.native_screen_width;
	global.camera.camera_rect.h = global.renderer_state.native_screen_height;

	global.camera.target = target;
}
void camera_update()
{
	// If a target is provided, we follow it
	if (global.camera.target != NULL)
	{
		global.camera.camera_rect.x =  global.camera.target->x +
			((global.renderer_state.screen_width - global.renderer_state.native_screen_width) / 2.0);
		global.camera.camera_rect.y = -global.camera.target->y +
			((global.renderer_state.screen_height - global.renderer_state.native_screen_height) / 2.0);
	}

	// Keeping the camera in the bounderies of the level
	if (global.camera.camera_rect.x < 0)
	{
		global.camera.camera_rect.x = 0;
	}
	if (global.camera.camera_rect.y < 0)
	{
		global.camera.camera_rect.y = 0;
	}
	if (global.camera.camera_rect.x > global.level_width - global.camera.camera_rect.w)
	{
		global.camera.camera_rect.x = global.level_width - global.camera.camera_rect.w;
	}
	if (global.camera.camera_rect.y > global.level_height - global.camera.camera_rect.h)
	{
		global.camera.camera_rect.y = global.level_height - global.camera.camera_rect.h;
	}
}


// External
u32 renderer_load_texture(char* path)
{
	return renderer_register_texture(path);
}
SDL_Texture* renderer_get_texture(u32 texture_id)
{
	return texture_stack.textures[texture_id];
}
void renderer_draw_sprite(int x, int y, Sprite sprite)
{
	if (sprite.texture == NULL)
	{
		printf("\nTexture is null, can't draw sprite");
		return;
	}

	SDL_SetTextureColorMod(sprite.texture, sprite.color.r, sprite.color.g, sprite.color.b);
	SDL_SetTextureAlphaMod(sprite.texture, sprite.color.a);

	f32 xpos = (x + global.renderer_state.screen_width / 2.0) - global.camera.camera_rect.x;
	f32 ypos = (-y + global.renderer_state.screen_height / 2.0) - global.camera.camera_rect.y;

	SDL_Rect position_rect = { .x = xpos, .y = ypos, sprite.rect.w, sprite.rect.h };

	SDL_RenderCopyEx(global.renderer_state.renderer,
			sprite.texture,
			&sprite.rect,
			&position_rect,
			sprite.rotation_angle,
			NULL,
			sprite.flip_mode);
}

Sprite renderer_create_sprite(u32 texture_id, SDL_Rect rect)
{
	Sprite sprite = { .texture = renderer_get_texture(texture_id), .rect = rect,
		.rotation_angle = 0.0,
		.blend_mode = SDL_BLENDMODE_BLEND,
		.flip_mode = SDL_FLIP_NONE };

	Sprite_Color color = { .r = 255, .g = 255, .b = 255, .a = 255 };
	sprite.color = color;

	SDL_SetTextureBlendMode(sprite.texture, sprite.blend_mode);

	SDL_SetTextureColorMod(sprite.texture, color.r, color.g, color.b);
	SDL_SetTextureAlphaMod(sprite.texture, color.a);

	return sprite;
}
void renderer_set_sprite_color(Sprite sprite, Sprite_Color color)
{
	sprite.color = color;
}
void renderer_flip_sprite(Sprite sprite, SDL_RendererFlip flip_mode)
{
	sprite.flip_mode = flip_mode;
}



// Internal

bool renderer_init(char* window_name, u32 native_width, u32 native_height, u32 window_width, u32 window_height, bool fullscreen)
{
	SDL_Window *window = NULL;

	// Initializing SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		ERROR_RETURN(false, "Couldn't initialize SDL: %s", SDL_GetError());
	}
	printf("\nSuccessfully Initialized SDL");

	// Creating an SDL window
	u32 window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS;
	if (fullscreen) window_flags |= SDL_WINDOW_FULLSCREEN;

	window = SDL_CreateWindow(window_name,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			window_width,
			window_height,
			window_flags);

	if (window == NULL)
	{
        ERROR_RETURN(false, "\nCould not create window: %s", SDL_GetError());
    }
	printf("\nSuccessfully Created an SDL window");


	global.renderer_state.window = window;

	global.renderer_state.screen_width = window_width;
	global.renderer_state.screen_height = window_height;

	global.renderer_state.native_screen_width = native_width;
	global.renderer_state.native_screen_height = native_height;

	// Creating the renderer
	global.renderer_state.renderer = SDL_CreateRenderer(global.renderer_state.window,
			-1,
			SDL_RENDERER_ACCELERATED);

	if (global.renderer_state.renderer == NULL)
	{
		ERROR_RETURN(false, "\nCouldn't create the renderer: %s", SDL_GetError());
	}
	printf("\nSuccessfully Created the renderer");

	// For Screen Scaling
	SDL_RenderSetLogicalSize(global.renderer_state.renderer,
			global.renderer_state.native_screen_width,
			global.renderer_state.native_screen_height);

	//Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        ERROR_RETURN(false, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}
	printf("\nSuccessfully Created the renderer");

	return true;
}

void renderer_shutdown(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyWindow(window); window = NULL;

	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	IMG_Quit();
}

bool renderer_textures_init()
{
	// Allocating memory for the texture stack
	texture_stack.textures = malloc(INITIAL_TEXTURE_COUNT * sizeof(SDL_Texture*));
	
	if (texture_stack.textures == NULL)
	{
		//Couldn't allocate memory for the textures
		printf("\nCouldn't allocate memory for the textures");
		return false;
	}

	texture_stack.count = 0;
	texture_stack.cap = INITIAL_TEXTURE_COUNT;

	return true;
}

u32 renderer_register_texture(char* path)
{
	// Maximum textures reached, reallocating memory
	if (texture_stack.count == texture_stack.cap)
	{
		size_t new_size = texture_stack.cap * 2 * sizeof(SDL_Texture*);
		texture_stack.textures = realloc(texture_stack.textures, new_size);
		if (texture_stack.textures == NULL)
		{
			// Couldn't allocate memory for textures
			printf("\nCouldn't reallocate memory for textures");
			return -1;
		}

		texture_stack.cap *= 2;
	}

	texture_stack.textures[texture_stack.count] = IMG_LoadTexture(global.renderer_state.renderer, path);
	texture_stack.count += 1;

	return texture_stack.count - 1;
}

void renderer_free_textures(void)
{
	for (int i = 0; i < texture_stack.count; i++)
	{
		SDL_DestroyTexture(texture_stack.textures[i]);
		texture_stack.textures[i] = NULL;
	}

	free(texture_stack.textures);
}

// ECS
void renderer_render_sprite_system(void)
{
	Ecs_Query_Result* qr = ecs_query(2, POSITION_COMPONENT, SPRITE_COMPONENT);

	for (int i = 0; i < qr->count; i++)
	{
		Position* pos = (Position*)ecs_get_component(qr->entities[i], POSITION_COMPONENT);
		Sprite* sprite = (Sprite*)ecs_get_component(qr->entities[i], SPRITE_COMPONENT);

		// Render
		renderer_draw_sprite(pos->x, pos->y, *sprite);
	}
}

