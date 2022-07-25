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

/**
 * Checks if a keyboard key was pressed in the previous frame
 *
 * @return returns true if the key was pressed in the previous frame, false if not
 */
bool key_was_down(SDL_Scancode key);
/**
 * Checks if a keyboard key is being pressed in the current frame
 *
 * @return returns true if the key is being pressed, false if not
 */
bool key_is_down(SDL_Scancode key);
/**
 * Checks if a keyboard key was up in the previous frame
 *
 * @return returns true if the key was up in the previous frame, false if not
 */
bool key_was_up(SDL_Scancode key);
/**
 * Checks if a keyboard key is up in the current frame
 *
 * @return returns true if the key is up, false if not
 */
bool key_is_up(SDL_Scancode key);

/**
 * Gets the x position of the mouse in screen space
 *
 * @return returns the x position of the mouse
 */
i16 get_mouse_screen_position_x();
/**
 * Gets the y position of the mouse in screen space
 *
 * @return returns the y position of the mouse
 */
i16 get_mouse_screen_position_y();

/**
 * Gets the x position of the mouse in world space
 *
 * @return returns the x position of the mouse
 */
i16 get_mouse_world_position_x();
/**
 * Gets the y position of the mouse in world space
 *
 * @return returns the y position of the mouse
 */
i16 get_mouse_world_position_y();

/**
 * Checks if a mouse button was pressed in the previous frame
 *
 * @return returns true if the button was pressed in the previous frame, false if not
 */
bool mouse_button_was_down(Mouse_Buttons button);
/**
 * Checks if a mouse button is being pressed in the current frame
 *
 * @return returns true if the button is being pressed, false if not
 */
bool mouse_button_is_down(Mouse_Buttons button);
/**
 * Checks if a mouse button was up in the previous frame
 *
 * @return returns true if the button was up in the previous frame, false if not
 */
bool mouse_button_was_up(Mouse_Buttons button);
/**
 * Checks if a mouse button is up in the current frame
 *
 * @return returns true if the button is up, false if not
 */
bool mouse_button_is_up(Mouse_Buttons button);
