# pragma once

# include "../types.h"
# include "../../../../include/SDL2/SDL_events.h"
# include "../../../../include/SDL2/SDL_mouse.h"

# ifndef MOUSE_BUTTON_DEFINDED
# define MOUSE_BUTTON_DEFINDED
typedef enum mouse_buttons
{
    MOUSE_BUTTON_LEFT,
	MOUSE_BUTTON_MIDDLE,
	MOUSE_BUTTON_RIGHT,
	MOUSE_BUTTON_MAX_BUTTONS

} Mouse_Buttons;
# endif

bool key_was_down(SDL_Scancode key);
bool key_is_down(SDL_Scancode key);
bool key_was_up(SDL_Scancode key);
bool key_is_up(SDL_Scancode key);

i16 get_mouse_position_x();
i16 get_mouse_position_y();

bool mouse_button_was_down(Mouse_Buttons button);
bool mouse_button_is_down(Mouse_Buttons button);
bool mouse_button_was_up(Mouse_Buttons button);
bool mouse_button_is_up(Mouse_Buttons button);
