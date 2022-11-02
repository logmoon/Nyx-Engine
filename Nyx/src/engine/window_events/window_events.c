# include <stdio.h>
# include "window_events.h"
# include "../input/input_internal.h"

bool pump_events(void)
{
	bool exit_flagged = false;
    SDL_Event event = {0};

    SDL_PollEvent(&event);
    switch(event.type)
    {
		case SDL_QUIT:
			exit_flagged = true;
			break;            
		// TODO: Other window events

		// Handing the user mouse and keyboard inputs
        default:
			handle_input_event(event);
            break;
    }

    return !exit_flagged;
}
