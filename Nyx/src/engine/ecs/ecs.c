# include <stdarg.h>
# include <stddef.h>
# include <stdlib.h>
# include <string.h>
# include "ecs.h"
# include "../utils.h"
# include "../global.h"

//TODO: remove this, and change refs to global.ecs_state

bool ecs_init(u32 component_count, ...)
{
	// Allocation memory for the stack
	global.ecs_state.stack = malloc(component_count);

	if (global.ecs_state.stack == NULL)
	{
		ERROR_RETURN(false, "\nCouldn't allocate memory for the ecs stack");
	}

	// Allocation memory for the component store
	global.ecs_state.component_store.capacity = malloc(component_count * sizeof(u32));
	global.ecs_state.component_store.top = malloc(component_count * sizeof(u32));
	global.ecs_state.component_store.sizes = malloc(component_count * sizeof(u32));

	if (global.ecs_state.component_store.sizes == NULL)
	{
		ERROR_RETURN(false, "\nCouldn't allocate memory for the components' sizes");
	}

	global.ecs_state.component_store.count = component_count;

	// Looping through the provided components and allocating memory for each component
	va_list ap;
	va_start(ap, component_count);
	for (int i = 0; i < component_count; ++i)
	{
		size_t component_size = va_arg(ap, size_t);
		
		global.ecs_state.component_store.sizes[i] = component_size;
		global.ecs_state.component_store.capacity[i] = ECS_INITIAL_COMPONENT_PER_ENTITY;
		global.ecs_state.component_store.top[i] = 0;

		global.ecs_state.stack[i] = malloc(ECS_INITIAL_COMPONENT_PER_ENTITY * component_size);

		if (global.ecs_state.stack[i] == NULL)
		{
			ERROR_RETURN(false, "\nCouldn't allocate memory for component of id: %d", i);
		}
	}
	va_end(ap);

	return true;
}

void ecs_shutdown(void)
{
	/*
	// Entities
	// Resetting
	global.ecs_state.entity_store.count = 0;
	global.ecs_state.entity_store.cap = 0;
	// Freeing resources
	free(global.ecs_state.entity_store.flags);
	global.ecs_state.entity_store.flags = NULL;
	free(global.ecs_state.entity_store.masks);
	global.ecs_state.entity_store.masks = NULL;

	// Query result
	free(global.ecs_state.query_result.entities);
	global.ecs_state.query_result.entities = NULL;
	*/

	// Components
	free(global.ecs_state.component_store.sizes);
	global.ecs_state.component_store.sizes = NULL;
	free(global.ecs_state.component_store.capacity);
	global.ecs_state.component_store.capacity = NULL;
	free(global.ecs_state.component_store.top);
	global.ecs_state.component_store.top = NULL;
	for (int i = 0; i < global.ecs_state.component_store.count; i++)
	{
		free(global.ecs_state.stack[i]);
		global.ecs_state.stack[i] = NULL;
	}
	global.ecs_state.component_store.count = 0;

	// Stack
	free(global.ecs_state.stack);
	global.ecs_state.stack = NULL;
}

bool ecs_scene_push(void)
{
	// Initializing the entity store, and allocating memory
	global.ecs_state.entity_store.count = 0;
	global.ecs_state.entity_store.cap = ECS_INITIAL_ENTITY_COUNT;
	global.ecs_state.entity_store.masks = malloc(ECS_INITIAL_ENTITY_COUNT * sizeof(u32));
	global.ecs_state.entity_store.flags = malloc(ECS_INITIAL_ENTITY_COUNT * sizeof(u32));

	// Initializing the query result
	global.ecs_state.query_result.count = 0;
	global.ecs_state.query_result.entities = malloc(ECS_INITIAL_ENTITY_COUNT * sizeof(u32));

	if (global.ecs_state.entity_store.masks == NULL ||
			global.ecs_state.entity_store.flags == NULL ||
			global.ecs_state.query_result.entities == NULL)
	{
		ERROR_RETURN(false, "\nCouldn't allocate memory for entites");
	}

	return true;
}
void ecs_scene_pop(void)
{
	// Entities
	// Resetting
	global.ecs_state.entity_store.count = 0;
	global.ecs_state.entity_store.cap = 0;
	// Freeing resources
	free(global.ecs_state.entity_store.flags);
	global.ecs_state.entity_store.flags = NULL;
	free(global.ecs_state.entity_store.masks);
	global.ecs_state.entity_store.masks = NULL;

	// Query result
	free(global.ecs_state.query_result.entities);
	global.ecs_state.query_result.entities = NULL;
}

Entity ecs_create_entity(void)
{
	u32 id = global.ecs_state.entity_store.count;

	// We have reached the end of the entity array, so we resize
	if (id == global.ecs_state.entity_store.cap)
	{
		printf("\n>> Max entities reached, reallocating");
		size_t new_size = 2 * global.ecs_state.entity_store.cap * sizeof(u32);
		printf("\n>> New Size: %u", ECS_ENTITY_EXPAND_MULTIPLIER * global.ecs_state.entity_store.cap);
		// Reallocating entity data
		global.ecs_state.entity_store.masks = realloc(global.ecs_state.entity_store.masks, new_size);
		global.ecs_state.entity_store.flags = realloc(global.ecs_state.entity_store.flags, new_size);
		// Reallocating ecs_query_result data
		global.ecs_state.query_result.entities = realloc(global.ecs_state.query_result.entities, new_size);

		if (global.ecs_state.entity_store.masks == NULL ||
				global.ecs_state.entity_store.flags == NULL ||
				global.ecs_state.query_result.entities == NULL)
		{
			// Couldn't reallocate memory
			printf("\nCouldn't reallocate memory");
			return -1;
		}
		global.ecs_state.entity_store.cap *= ECS_ENTITY_EXPAND_MULTIPLIER;
	}

	global.ecs_state.entity_store.count++;
	global.ecs_state.entity_store.masks[id] = 0;
	global.ecs_state.entity_store.flags[id] = ENTITY_FLAG_ALIVE;

	return id;
}

void* ecs_get_component(Entity entity_id, u32 component_id)
{
	return (u8*)global.ecs_state.stack[component_id] + (global.ecs_state.component_store.sizes[component_id] * entity_id);
}

bool ecs_has_component(Entity entity_id, u32 component_id)
{
	return (global.ecs_state.entity_store.masks[entity_id] & (1 << component_id)) != 0;
}

bool ecs_add_component(Entity entity_id, u32 component_id, void* data)
{
	if (global.ecs_state.stack[component_id] == NULL)
	{
		// Component wasn't initialized, check the provided id
		printf("\n>> Component wasn't initialized, check the provided id");
		return false;
	}

	if (ecs_has_component(entity_id, component_id))
	{
		// Already has the component
		printf("\n>> Entity:%u already has the component:%u", entity_id, component_id);
		return false;
	}

	// Checking if we are out of memory, if so we reallocate
	if (global.ecs_state.component_store.capacity[component_id] == global.ecs_state.component_store.top[component_id])
	{
		size_t new_size = global.ecs_state.component_store.capacity[component_id] *
			ECS_COMPONENT_EXPAND_MULTIPLIER *
			global.ecs_state.component_store.sizes[component_id];

		global.ecs_state.stack[component_id] = realloc(global.ecs_state.stack[component_id], new_size);

		if (global.ecs_state.stack[component_id] == NULL)
		{
			// Couldn't reallocate memory
			printf("\n>> Couldn't reallocate memory for component data");
			return false;
		}
		global.ecs_state.component_store.capacity[component_id] *= ECS_COMPONENT_EXPAND_MULTIPLIER;
	}

	size_t component_size = global.ecs_state.component_store.sizes[component_id];
	void* dest = ecs_get_component(entity_id, component_id);

	if (memcpy(dest, data, component_size) == NULL)
	{
		// Couldn't copy component data to memory
		printf("\n>> Couldn't copy component data to memory");
		return false;
	}

	global.ecs_state.entity_store.masks[entity_id] |= (1 << component_id);
	global.ecs_state.component_store.top[component_id]++;

	return true;
}

void ecs_remove_component(Entity entity_id, u32 component_id)
{
	global.ecs_state.entity_store.masks[entity_id] &= ~(1 << component_id);
	// For now, I'm not freeing the entity's data from the components
}

void ecs_kill_entity(Entity entity_id)
{
	// Check if it is alive
	if ((global.ecs_state.entity_store.flags[entity_id] & ENTITY_FLAG_ALIVE) != 0)
	{
		// Set it to dead
		global.ecs_state.entity_store.flags[entity_id] &= ~ENTITY_FLAG_ALIVE;
		// Reset the mask
		global.ecs_state.entity_store.masks[entity_id] = 0;
	}
	else
	{
		// Entity already dead
		return;
	}
}

Ecs_Query_Result* ecs_query(u32 component_count, ...)
{
	va_list ap;
	u32 i, mask = 0;

	global.ecs_state.query_result.count = 0;

	va_start(ap, component_count);
	for (i = 0; i < component_count; ++i)
	{
		mask |= (1 << va_arg(ap, u32));
	}
	va_end(ap);

	for(i = 0; i < global.ecs_state.entity_store.count; ++i)
	{
		if ((global.ecs_state.entity_store.flags[i] & ENTITY_FLAG_ALIVE) != 0 &&
				mask == (global.ecs_state.entity_store.masks[i] & mask))
		{
			global.ecs_state.query_result.entities[global.ecs_state.query_result.count++] = i;
		}
	}

	return &global.ecs_state.query_result;
}

