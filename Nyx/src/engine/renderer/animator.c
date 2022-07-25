# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include "animator_internal.h"
# include "animator.h"
# include "../ecs/ecs.h"

// Internal
Animator_Stack stack;

bool animator_init(void)
{
	if (stack.initialized)
	{
		printf("\nAnimator stack is already initialized");
		return true;
	}

	stack.initialized = false;

	stack.animators = malloc(ANIMATOR_INITIAL_ANIMATORS * sizeof(Animator));

	if (stack.animators == NULL)
	{
		// Couldn't allocate memory for animators
		printf("\nCouldn't allocate memory for animators");
		return false;
	}

	stack.count = 0;
	stack.cap = ANIMATOR_INITIAL_ANIMATORS;
	stack.initialized = true;

	return true;
}
void animator_free(void)
{
	u32 i;

	for (i = 0; i < stack.count; i++)
	{
		free(stack.animators[i].animations);
		stack.animators[i].animations = NULL;

		stack.animators[i].animation_count = 0;
		stack.animators[i].animation_cap = 0;
		stack.animators[i].current_animation = -1;
		stack.animators[i].animation_changed = false;
		stack.animators[i].frame_increase = 1;
		stack.animators[i].current_frame = 0;
		stack.animators[i].animation_timer = 0;
	}

	free(stack.animators);
	stack.animators = NULL;
	stack.count = 0;
	stack.cap = 0;
	stack.initialized = false;
}
// ECS
void animator_animate_system(void)
{
	Ecs_Query_Result* qr = ecs_query(2, SPRITE_COMPONENT, ANIMATOR_COMPONENT);

	for (int i = 0; i < qr->count; i++)
	{
		Sprite* sprite = (Sprite*)ecs_get_component(qr->entities[i], SPRITE_COMPONENT);
		Animator* animator = (Animator*)ecs_get_component(qr->entities[i], ANIMATOR_COMPONENT);

		// Animate
		animator_animate(animator, &sprite->rect);
	}
}


// External
Animator* animator_create_animator(void)
{
	if (!stack.initialized)
	{
		printf("\nThe animator stack is not initialized, can't create animator");
		return NULL;
	}

	// Checking if the stack is full, if so we reallocate more memory for it
	if (stack.count == stack.cap)
	{
		size_t new_size = stack.cap * 2 * sizeof(Animator);
		stack.animators = realloc(stack.animators, new_size);

		if (stack.animators == NULL)
		{
			// Reallocation failed
			printf("\nCouldn't reallocate memory for the animator stack");
			return NULL;
		}

		stack.cap *= 2;
	}

	Animator animator;

	animator.animations = malloc(ANIMATOR_INITIAL_ANIMATION * sizeof(Animation));

	if (animator.animations == NULL)
	{
		// Couldn't allocate memory for the animations
		printf("\nCouldn't allocate memory for the animations");
		return NULL;
	}

	animator.animation_count = 0;
	animator.animation_cap = ANIMATOR_INITIAL_ANIMATION;

	animator.current_animation = -1;
	animator.animation_changed = false;

	animator.frame_increase = 1;
	animator.current_frame = 0;

	animator.animation_timer = 0;

	// Add the created animator to the stack
	stack.animators[stack.count] = animator;
	stack.count++;

	return &stack.animators[stack.count - 1];
}


u32 animator_add_animation(Animator* animator,
		SDL_Rect first_frame,
		u32 frame_count,
		u32 frame_rate,
		Animation_Type animation_type)
{
	if (animator->animation_count == animator->animation_cap)
	{
		// animation stack is full, need to resise
		animator->animations = realloc(animator->animations,
				animator->animation_cap * 2 * sizeof(Animation));
		if (animator->animations == NULL)
		{
			// couldn't reallocate memory
			return -1;
		}

		animator->animation_cap *= 2;
	}

	animator->animations[animator->animation_count].first_frame = first_frame;
	animator->animations[animator->animation_count].frame_count = frame_count;
	animator->animations[animator->animation_count].frame_rate = frame_rate;
	animator->animations[animator->animation_count].animation_type = animation_type;

	animator->animation_count += 1;

	return animator->animation_count - 1;
}

void animator_set_active_animation(Animator* animator, u32 animation)
{
	animator->current_animation = animation;
	animator->animation_changed = true;
}


void animator_animate(Animator* animator, SDL_Rect* rect)
{
	if (animator->animation_changed)
	{
		animator->animation_timer = 0;

		rect->x = animator->animations[animator->current_animation].first_frame.x;
		rect->y = animator->animations[animator->current_animation].first_frame.y;

		animator->frame_increase = 1;
		animator->current_frame = 0;

		animator->animation_changed = false;
	}

	u32 animation = animator->current_animation;

	if (animator->animation_timer + animator->animations[animation].frame_rate > SDL_GetTicks())
	{
		return;
	}

	animator->animation_timer = SDL_GetTicks();


	switch (animator->animations[animation].animation_type)
	{
		case NO_LOOP:
			if(animator->current_frame < animator->animations[animation].frame_count)
			{
				rect->x = animator->animations[animation].first_frame.w * animator->current_frame;
			}
			break;
		case FORWARD:
			if(animator->current_frame >= animator->animations[animation].frame_count)
			{
				rect->x = animator->animations[animator->current_animation].first_frame.x;
				animator->current_frame = 0;
			}
			else
			{
				rect->x = animator->animations[animation].first_frame.w * animator->current_frame;
			}
			break;
		case PING_PONG:
			if (animator->frame_increase > 0)
			{
				if (animator->current_frame >= animator->animations[animation].frame_count)
				{
					animator->frame_increase *= -1;
				}
				else
				{
					rect->x = animator->animations[animation].first_frame.w * animator->current_frame;
				}
			}
			else
			{
				if (animator->current_frame <= 0)
				{
					animator->frame_increase *= -1;
				}
				rect->x = animator->animations[animation].first_frame.w * animator->current_frame;
			}

			break;
		default:
			return;

	}
	
	animator->current_frame += animator->frame_increase;
}
