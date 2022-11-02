# pragma once
# include "../../engine/base_components/base_components.h"
# include "../../engine/types.h"

typedef void(*menu_item_action)(void);

typedef struct menu_item
{
	Position position;
	Text text;
	menu_item_action action;

}Menu_Item;

typedef struct menu
{
	u8 selected_item;
	u8 item_count;
	Menu_Item** items;

}Menu;


Menu* menu_create_menu(u32 menu_item_count, ...);
Menu_Item menu_create_menu_item(Position position, Text text, void* action);
void menu_navigate_up(Menu* menu, bool loop);
void menu_navigate_down(Menu* menu, bool loop);
menu_item_action menu_get_selected_menu_item_action(Menu* menu);
Menu_Item* menu_get_selected_menu_item(Menu* menu);
void menu_free_menu(Menu* menu);
