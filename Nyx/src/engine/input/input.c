# include <stdbool.h>
# include "input.h"
# include "input_internal.h"

typedef struct keyboard_state
{
    bool keys[256];

} Keyboard_State;

typedef struct mouse_state
{
    i16 x;
    i16 y;
    bool buttons[MOUSE_BUTTON_MAX_BUTTONS];

} Mouse_State;

typedef struct input_state
{
    Keyboard_State keyboard_current;
    Keyboard_State keyboard_previous;
    Mouse_State mouse_current;
    Mouse_State mouse_previous;

} Input_State;

Input_State input_state = {0};

void handle_input_event(SDL_Event event)
{
    input_state.keyboard_previous = input_state.keyboard_current;
    input_state.mouse_previous = input_state.mouse_current;

    switch(event.type)
    {
		// Mouse And Keyboard Presses
        case SDL_KEYDOWN:
            input_state.keyboard_current.keys[event.key.keysym.sym] = true;
        break;
        case SDL_KEYUP:
            input_state.keyboard_current.keys[event.key.keysym.sym] = false;
        break;
        case SDL_MOUSEMOTION:
            input_state.mouse_current.x = event.motion.x;
            input_state.mouse_current.y = event.motion.y;
        break;
        case SDL_MOUSEBUTTONDOWN:
            input_state.mouse_current.buttons[event.button.button - 1] = true;
        break;
        case SDL_MOUSEBUTTONUP:
            input_state.mouse_current.buttons[event.button.button - 1] = false;
        break;
    }
}


bool key_was_down(SDL_KeyCode key)
{
	return input_state.keyboard_previous.keys[key];
}
bool key_is_down(SDL_KeyCode key)
{
	return input_state.keyboard_current.keys[key];
}
bool key_was_up(SDL_KeyCode key)
{
	return !input_state.keyboard_previous.keys[key];
}
bool key_is_up(SDL_KeyCode key)
{
	return !input_state.keyboard_current.keys[key];
}

i16 get_mouse_position_x()
{
	return input_state.mouse_current.x;
}
i16 get_mouse_position_y()
{
	return input_state.mouse_current.y;
}

bool mouse_button_was_down(Mouse_Buttons button)
{
	return input_state.mouse_previous.buttons[button];
}
bool mouse_button_is_down(Mouse_Buttons button)
{
	return input_state.mouse_current.buttons[button];
}
bool mouse_button_was_up(Mouse_Buttons button)
{
	return !input_state.mouse_previous.buttons[button];
}
bool mouse_button_is_up(Mouse_Buttons button)
{
	return !input_state.mouse_current.buttons[button];
}

