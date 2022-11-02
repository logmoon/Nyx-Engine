# include "main_scene.h"
# include "scene.h"
# include "../../engine/menu/menu.h"
# include "../../engine/ecs/ecs.h"
# include "../../engine/global.h"
# include "../../engine/utils.h"
# include "../../engine/core/core.h"
# include "../../engine/audio/audio.h"
# include "../../engine/input/input.h"
# include "../../engine/renderer/renderer.h"
# include "../../engine/renderer/camera.h"
# include "../../engine/renderer/animator.h"
# include "../../engine/base_components/base_components.h"
# include "../../engine/scene_manager/scene_manager.h"

Scene game_scene;
Menu* main_menu;

void play_button_pressed(void)
{
	set_active_scene(&game_scene);
}

void exit_button_pressed(void)
{
	core_exit_application();
}

void main_scene_init(void)
{
	global_set_level_dimentions(426, 240);
	game_scene = create_scene(&scene_init, &scene_update, &scene_render, &scene_shutdown);

	Entity game_title = ecs_create_entity();
	Position title_position = { .x = 0, .y = 90 };
	Text title = renderer_load_text_texture("DownBeat", 1.0, true);
	ecs_add_component(game_title, POSITION_COMPONENT, &title_position);
	ecs_add_component(game_title, TEXT_COMPONENT, &title);

	Entity player = ecs_create_entity();
	Position player_pos = { .x = 0, .y = 0 };
	u32 player_texture = renderer_load_sprite_texture("Assets/Characters/player_free.png");
	Rect player_rect = { .x = 0, .y = 0, .w = 16, .h = 16 };
	Sprite player_spr = renderer_create_sprite(player_texture, player_rect, 0, 0.7);
	ecs_add_component(player, POSITION_COMPONENT, &player_pos);
	ecs_add_component(player, SPRITE_COMPONENT, &player_spr);

	Texture_Color red = { .r = 255, .g = 0, .b = 0, .a = 255 };
	Sprite* spr = (Sprite*)ecs_get_component(1, SPRITE_COMPONENT);
	spr->color = red;

	Position play_button_pos = { .x = 0, .y = 0 };
	Text play_button_text = renderer_load_text_texture("Play", 0.5, true);
	Menu_Item play_button = menu_create_menu_item(play_button_pos, play_button_text, &play_button_pressed);

	Position exit_button_pos = { .x = 0, .y = -25 };
	Text exit_button_text = renderer_load_text_texture("Quit", 0.5, true);
	Menu_Item exit_button = menu_create_menu_item(exit_button_pos, exit_button_text, &exit_button_pressed);

	main_menu = menu_create_menu(2, play_button, exit_button);

	Position* cursor_pos = (Position*)ecs_get_component(1, POSITION_COMPONENT);
	cursor_pos->x = -(menu_get_selected_menu_item(main_menu)->text.width / 3.0);
	cursor_pos->y = menu_get_selected_menu_item(main_menu)->position.y;
}
void main_scene_update(f32 time, f32 delta_time)
{
	if (key_is_down(SDL_SCANCODE_W) && !key_was_down(SDL_SCANCODE_W))
	{
		menu_navigate_up(main_menu, true);
		Position* cursor_pos = (Position*)ecs_get_component(1, POSITION_COMPONENT);

	 	cursor_pos->x = -(menu_get_selected_menu_item(main_menu)->text.width / 3.0);
	 	cursor_pos->y = menu_get_selected_menu_item(main_menu)->position.y;
	}

	if (key_is_down(SDL_SCANCODE_S) && !key_was_down(SDL_SCANCODE_S))
	{
		menu_navigate_down(main_menu, true);
		Position* cursor_pos = (Position*)ecs_get_component(1, POSITION_COMPONENT);

	 	cursor_pos->x = -(menu_get_selected_menu_item(main_menu)->text.width / 3.0);
	 	cursor_pos->y = menu_get_selected_menu_item(main_menu)->position.y;
	}

	if (key_is_down(SDL_SCANCODE_RETURN) && !key_was_down(SDL_SCANCODE_RETURN))
	{
		if (menu_get_selected_menu_item_action(main_menu) != NULL)
		{
			menu_get_selected_menu_item_action(main_menu)();
		}
	}
}
void main_scene_render(void)
{
}
void main_scene_shutdown(void)
{
	menu_free_menu(main_menu);
}
