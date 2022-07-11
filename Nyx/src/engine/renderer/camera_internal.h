# pragma once

# include "../../../../include/SDL2/SDL.h"
# include "../base_components/base_components.h"

typedef struct camera
{
	SDL_Rect camera_rect;
	Position* target;

}Camera;

void camera_update();
