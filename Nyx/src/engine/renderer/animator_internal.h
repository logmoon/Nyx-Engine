# pragma once
# include "../base_components/base_components.h"

# define ANIMATOR_INITIAL_ANIMATORS 5

typedef struct animator_stack
{
	Animator* animators;
	bool initialized;
	u32 count;
	u32 cap;

}Animator_Stack;


bool animator_init(void);
void animator_free(void);
void animator_animate_system(void);
