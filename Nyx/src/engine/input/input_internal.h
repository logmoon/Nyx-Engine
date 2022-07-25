# pragma once

# include "../types.h"
# include "../../../../include/SDL2/SDL_events.h"

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


/**
 * Handles the user's inputs
 *
 */
void handle_input_event(SDL_Event event);
