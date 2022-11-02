# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include "menu.h"
# include "../ecs/ecs.h"

Menu* menu_create_menu(u32 menu_item_count, ...)
{
	Menu* menu = calloc(1, sizeof(Menu));

	menu->items = calloc(menu_item_count, sizeof(Menu_Item*));
	menu->item_count = menu_item_count;
	menu->selected_item = 0;

	va_list ap;
	va_start(ap, menu_item_count);
	for (int i = 0; i < menu_item_count; i++)
	{
		Menu_Item item = va_arg(ap, Menu_Item);

		Entity e = ecs_create_entity();
		ecs_add_component(e, POSITION_COMPONENT, &item.position);
		ecs_add_component(e, TEXT_COMPONENT, &item.text);

		menu->items[i] = calloc(1, sizeof(Menu_Item));

		menu->items[i]->position = item.position;
		menu->items[i]->text = item.text;
		menu->items[i]->action = item.action;
	}
	va_end(ap);

	return menu;
}

Menu_Item menu_create_menu_item(Position position, Text text, void* action)
{
	Menu_Item item = { .position = position, .text = text, .action = action };
	return item;
}

void menu_navigate_up(Menu* menu, bool loop)
{
	if (menu->selected_item <= 0)
	{
		if (loop) menu->selected_item = menu->item_count - 1;

		return;
	}

	menu->selected_item--;
}

void menu_navigate_down(Menu* menu, bool loop)
{
	if (menu->selected_item >= menu->item_count - 1)
	{
		if (loop) menu->selected_item = 0;

		return;
	}

	menu->selected_item++;
}

menu_item_action menu_get_selected_menu_item_action(Menu* menu)
{
	return menu->items[menu->selected_item]->action;
}

Menu_Item* menu_get_selected_menu_item(Menu* menu)
{
	return menu->items[menu->selected_item];
}

void menu_free_menu(Menu* menu)
{
	for (u32 i  = 0; i < menu->item_count; i++)
	{
		free(menu->items[i]);
		menu->items[i] = NULL;
	}

	free(menu->items);
	menu->items = NULL;
	free(menu);
	menu = NULL;
}
