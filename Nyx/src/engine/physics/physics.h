# pragma once
# include "../types.h"

# define GRAVITY -1000
# define PHYSICS_SUB_STEPS 4

typedef struct velet_object
{
	Vector2 current_position;
	Vector2 previous_position;
	Vector2 acceleration;

}Velet_Object;

typedef struct rigidbody
{
	Velet_Object obj;
	f32 gravity_scale;

}Rigidbody;

typedef struct circle_collider
{
	f32 radius;

}Circle_Collider;

typedef struct constraint
{
	Vector2 position;
	bool horizontal;
	f32 len;

}Constraint;

Rigidbody physics_create_rigidbody(f32 gravity_scale);
Circle_Collider physics_create_circle_collider(f32 radius);
void update_position(Velet_Object* obj, f32 dt);
void accelerate(Velet_Object* obj, Vector2 acceleration);
void physics_cirlce_collider_solve(Velet_Object* obj1, Circle_Collider collider1, Velet_Object* obj2, Circle_Collider collider2);
void physics_apply_gravity_system();
void physics_solve_collision_system();
void physics_update_positions_system(f32 dt);
