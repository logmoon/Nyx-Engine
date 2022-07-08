# pragma once
# include "../types.h"

typedef u32 Entity;

typedef struct entity_store
{
	u32* masks;
	u32* flags;
	u32 count;
	u32 cap;

}Entity_Store;

typedef struct component_store
{
	u32 count;
	u32* sizes;
	u32* capacity;
	u32* top;

}Component_Store;

typedef struct ecs_query_result
{
	u32 count;
	Entity* entities;

}Ecs_Query_Result;

typedef struct ecs_state
{
	void** stack;
	Entity_Store entity_store;
	Component_Store component_store;
	Ecs_Query_Result query_result;

}Ecs_State;


# define ENTITY_FLAG_ALIVE 1

# define ECS_INITIAL_COMPONENT_PER_ENTITY 10
# define ECS_COMPONENT_EXPAND_MULTIPLIER 2

# define ECS_INITIAL_ENTITY_COUNT 10
# define ECS_ENTITY_EXPAND_MULTIPLIER 2


bool ecs_init(u32 component_count, ...);
void ecs_shutdown(void);

bool ecs_scene_push(void);
void ecs_scene_pop(void);

Entity ecs_create_entity(void);
void* ecs_get_component(Entity entity_id, u32 component_id);
bool ecs_has_component(Entity entity_id, u32 component_id);
bool ecs_add_component(Entity entity_id, u32 component_id, void* data);
void ecs_remove_component(Entity entity_id, u32 component_id);
void ecs_kill_entity(Entity entity_id);
Ecs_Query_Result* ecs_query(u32 component_count, ...);
