# pragma once 
# include "../base_components/base_components.h"

# define ANIMATOR_INITIAL_ANIMATION 10

Animator* animator_create_animator(void);
u32 animator_add_animation(Animator* animator,
		SDL_Rect first_frame,
		u32 frame_count,
		u32 frame_rate,
		Animation_Type animation_type);
void animator_set_active_animation(Animator* animator, u32 animation);
void animator_animate(Animator* animator, SDL_Rect* rect);
