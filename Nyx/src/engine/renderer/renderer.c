# include "renderer_internal.h"
# include "renderer.h"
# include "camera_internal.h"
# include "camera.h"
# include "../ecs/ecs.h"
# include "../base_components/base_components.h"
# include "../global.h"
# include "../utils.h"
# include "../../../../include/SDL2/SDL_image.h"
# include "../../../../include/SDL2/SDL_ttf.h"


Texture_Stack texture_stack;
TTF_Font* font;

// Camera
void camera_init(Position* target, f32 camera_size)
{
	global.camera.camera_rect.x = (global.level_width - global.renderer_state.native_screen_width) / 2.0;
	global.camera.camera_rect.y = (global.level_height - global.renderer_state.native_screen_height) / 2.0;
	
	global.camera.camera_rect.w = global.renderer_state.native_screen_width * camera_size;
	global.camera.camera_rect.h = global.renderer_state.native_screen_height * camera_size;

	global.camera.target = target;
}
void camera_update(void)
{
	// If a target is provided, we follow it
	if (global.camera.target != NULL)
	{
		global.camera.camera_rect.x =  global.camera.target->x +
			((global.level_width - global.renderer_state.native_screen_width) / 2.0);
		global.camera.camera_rect.y = -global.camera.target->y +
			((global.level_height - global.renderer_state.native_screen_height) / 2.0);
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
SDL_Texture* renderer_get_texture(u32 texture_id)
{
	return texture_stack.textures[texture_id];
}


u32 renderer_load_sprite_texture(char* path)
{
	SDL_Texture* t = IMG_LoadTexture(global.renderer_state.renderer, path);
	if (t == NULL)
	{
		printf("\nCouldn't load image texture, %s", IMG_GetError());
		return -1;
	}

	return renderer_register_texture(t);
}
bool renderer_load_font(char* font_path, u32 font_size)
{
	if (font != NULL)
	{
		printf("\nA font is already loaded");
		TTF_CloseFont(font);
		font = NULL;
	}

	font = TTF_OpenFont(font_path, font_size);

	if (font == NULL)
	{
		printf("\nCouldn't load font, %s", TTF_GetError());
		return false;
	}

	return true;
}
Text renderer_load_text_texture(char* text, f32 text_size, bool is_ui)
{
	Text data = { 0 };

	if (font == NULL)
	{
		printf("\nNo font is currently loaded, can't load the text");
		return data;
	}

	SDL_Color c = { .r = 255, .g = 255, .b = 255 };
	SDL_Surface* surface_text =	TTF_RenderText_Solid(font, text, c);
	u32 id = renderer_register_texture(SDL_CreateTextureFromSurface(global.renderer_state.renderer, surface_text));
	SDL_FreeSurface(surface_text);

	int w = 0;
	int h = 0;
	Texture_Color color = { .r = 255, .g = 255, .b = 255, .a = 255 };
	TTF_SizeUTF8(font, text, &w, &h);

	data.texture_id = id;
	data.color = color;
	data.width = w;
	data.height = h; 
	data.size = text_size;
	data.is_ui = is_ui;

	return data;
}
Image renderer_load_image_texture(char* path, f32 image_size, bool is_ui)
{
	Image data = { 0 };

	SDL_Color c = { .r = 255, .g = 255, .b = 255 };
	u32 id = renderer_register_texture(IMG_LoadTexture(global.renderer_state.renderer,
				path));

	int w = 0;
	int h = 0;
	Texture_Color color = { .r = 255, .g = 255, .b = 255, .a = 255 };
	SDL_QueryTexture(renderer_get_texture(id), NULL, NULL, &w, &h);

	data.texture_id = id;
	data.color = color;
	data.width = w;
	data.height = h; 
	data.size = image_size;
	data.is_ui = is_ui;

	return data;
}


Sprite renderer_create_sprite(u32 texture_id, SDL_Rect rect, f32 sprite_size)
{
	Sprite sprite = { .texture_id = texture_id,
		.rect = rect,
		.rotation_angle = 0.0,
		.flip_mode = SDL_FLIP_NONE,
		.size = sprite_size};



	Texture_Color color = { .r = 255, .g = 255, .b = 255, .a = 255 };
	sprite.color = color;

	SDL_SetTextureBlendMode(renderer_get_texture(sprite.texture_id), SDL_BLENDMODE_BLEND);

	SDL_SetTextureColorMod(renderer_get_texture(sprite.texture_id), color.r, color.g, color.b);
	SDL_SetTextureAlphaMod(renderer_get_texture(sprite.texture_id), color.a);

	return sprite;
}
void renderer_draw_sprite(int x, int y, Sprite sprite)
{
	if (renderer_get_texture(sprite.texture_id) == NULL)
	{
		printf("\nTexture is null, can't draw sprite");
		return;
	}

	SDL_SetTextureColorMod(renderer_get_texture(sprite.texture_id), sprite.color.r, sprite.color.g, sprite.color.b);
	SDL_SetTextureAlphaMod(renderer_get_texture(sprite.texture_id), sprite.color.a);

	f32 xpos = (x - ((sprite.rect.w * sprite.size) / 2.0) + (global.level_width / 2.0)) - global.camera.camera_rect.x;
	f32 ypos = (-(y + ((sprite.rect.h * sprite.size) / 2.0)) + (global.level_height / 2.0)) - global.camera.camera_rect.y;

	SDL_Rect position_rect = { .x = xpos, .y = ypos, sprite.rect.w * sprite.size, sprite.rect.h * sprite.size };

	SDL_RenderCopyEx(global.renderer_state.renderer,
			renderer_get_texture(sprite.texture_id),
			&sprite.rect,
			&position_rect,
			sprite.rotation_angle,
			NULL,
			sprite.flip_mode);
}
void renderer_set_sprite_color(Sprite sprite, Texture_Color color)
{
	sprite.color = color;
}
void renderer_flip_sprite(Sprite sprite, SDL_RendererFlip flip_mode)
{
	sprite.flip_mode = flip_mode;
}
void renderer_set_sprite_size(Sprite sprite, f32 size)
{
	sprite.size = size;
}


void renderer_draw_text(int x, int y, Text data)
{
	if (renderer_get_texture(data.texture_id) == NULL)
	{
		printf("\nTexture is null, can't draw text");
		return;
	}

	SDL_SetTextureColorMod(renderer_get_texture(data.texture_id), data.color.r, data.color.g, data.color.b);
	SDL_SetTextureAlphaMod(renderer_get_texture(data.texture_id), data.color.a);

	f32 xpos = x - ((data.width * data.size) / 2.0) + (global.renderer_state.native_screen_width / 2.0);
	f32 ypos = -(y + ((data.height * data.size) / 2.0)) + (global.renderer_state.native_screen_height / 2.0);
	if (!data.is_ui)
	{
		xpos = ((x - ((data.width * data.size) / 2.0)) + (global.level_width / 2.0)) - global.camera.camera_rect.x;
		ypos = (-(y + ((data.height * data.size) / 2.0)) + (global.level_height / 2.0)) - global.camera.camera_rect.y;
	}
	
	SDL_Rect src_rest = { .x = 0, .y = 0, .w = data.width, .h = data.height };
	SDL_Rect position_rect = { .x = xpos, .y = ypos, data.width * data.size, data.height * data.size };
	SDL_RenderCopy(global.renderer_state.renderer, renderer_get_texture(data.texture_id),
			&src_rest,
			&position_rect);
}
void renderer_set_text_color(Text data, Texture_Color color)
{
	data.color = color;
}
void renderer_set_text_size(Text data, f32 size)
{
	data.size = size;
}


void renderer_draw_image(int x, int y, Image data)
{
	if (renderer_get_texture(data.texture_id) == NULL)
	{
		printf("\nTexture is null, can't draw text");
		return;
	}

	SDL_SetTextureColorMod(renderer_get_texture(data.texture_id), data.color.r, data.color.g, data.color.b);
	SDL_SetTextureAlphaMod(renderer_get_texture(data.texture_id), data.color.a);

	f32 xpos = x - ((data.width * data.size) / 2.0) + (global.renderer_state.native_screen_width / 2.0);
	f32 ypos = -(y + ((data.height * data.size) / 2.0)) + (global.renderer_state.native_screen_height / 2.0);
	if (!data.is_ui)
	{
		xpos = ((x - ((data.width * data.size) / 2.0)) + (global.level_width / 2.0)) - global.camera.camera_rect.x;
		ypos = (-(y + ((data.height * data.size) / 2.0)) + (global.level_height / 2.0)) - global.camera.camera_rect.y;
	}
	
	SDL_Rect src_rest = { .x = 0, .y = 0, .w = data.width, .h = data.height };
	SDL_Rect position_rect = { .x = xpos, .y = ypos, data.width * data.size, data.height * data.size };
	SDL_RenderCopy(global.renderer_state.renderer, renderer_get_texture(data.texture_id),
			&src_rest,
			&position_rect);

}
void renderer_set_image_color(Image data, Texture_Color color)
{
	data.color = color;
}
void renderer_set_image_size(Image data, f32 size)
{
	data.size = size;
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
	
	/*
	//Set texture filtering to linear
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"))
    {
        printf("\nLinear texture filtering not enabled");
    }
	printf("\nSuccessfully enabled linear texture filtering");
	*/

	//Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        ERROR_RETURN(false, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}
	printf("\nSuccessfully Initialized SDL_image");

	if (TTF_Init() == -1)
	{
		printf("\nCouldn't initilize SDL_ttf, Error:%s", TTF_GetError());
		return false;
	}
	printf("\nSuccessfully Initialized SDL_ttf");

	return true;
}

void renderer_shutdown(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyWindow(window); window = NULL;

	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	IMG_Quit();

	TTF_CloseFont( font );
    font = NULL;
	TTF_Quit();
}

bool renderer_textures_init(void)
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

u32 renderer_register_texture(SDL_Texture* texture)
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

	// texture_stack.textures[texture_stack.count] = IMG_LoadTexture(global.renderer_state.renderer, path);
	texture_stack.textures[texture_stack.count] = texture;
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
void renderer_render_sprites_system(void)
{
	u32 i = 0;
	Ecs_Query_Result* sprite_qr = ecs_query(2, POSITION_COMPONENT, SPRITE_COMPONENT);

	// Rendering Sprites
	for (i = 0; i < sprite_qr->count; i++)
	{
		Position* pos = (Position*)ecs_get_component(sprite_qr->entities[i], POSITION_COMPONENT);
		Sprite* sprite = (Sprite*)ecs_get_component(sprite_qr->entities[i], SPRITE_COMPONENT);

		renderer_draw_sprite(pos->x, pos->y, *sprite);
	}
}
void renderer_render_text_system(void)
{
	u32 i = 0;
	Ecs_Query_Result* text_qr = ecs_query(2, POSITION_COMPONENT, TEXT_COMPONENT);

	// Rendering Text
	for (i = 0; i < text_qr->count; i++)
	{
		Position* pos = (Position*)ecs_get_component(text_qr->entities[i], POSITION_COMPONENT);
		Text* text = (Text*)ecs_get_component(text_qr->entities[i], TEXT_COMPONENT);

		renderer_draw_text(pos->x, pos->y, *text);
	}
}
void renderer_render_images_system(void)
{
	u32 i = 0;
	Ecs_Query_Result* image_qr = ecs_query(2, POSITION_COMPONENT, IMAGE_COMPONENT);

	// Rendering Images
	for (i = 0; i < image_qr->count; i++)
	{
		Position* pos = (Position*)ecs_get_component(image_qr->entities[i], POSITION_COMPONENT);
		Image* image = (Image*)ecs_get_component(image_qr->entities[i], IMAGE_COMPONENT);

		renderer_draw_image(pos->x, pos->y, *image);
	}
}
