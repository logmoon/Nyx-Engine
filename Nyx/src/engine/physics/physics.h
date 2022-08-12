# pragma once
# include "../types.h"

# define GRAVITY -1000
# define PHYSICS_SUB_STEPS 1

typedef struct verlet_object
{
	Vector2 current_position;
	Vector2 previous_position;
	Vector2 acceleration;

}Verlet_Object;

typedef struct rigidbody
{
	Verlet_Object obj;
	f32 gravity_scale;

}Rigidbody;

typedef struct circle_collider
{
	f32 radius;

}Circle_Collider;

typedef enum constraint_type
{
	horizontal,
	vertical
} Constraint_Type;
typedef struct constraint
{
	Constraint_Type type;
	i32 len;

}Constraint;

Rigidbody physics_create_rigidbody(f32 starting_x_pos, f32 starting_y_pos, f32 gravity_scale);
Circle_Collider physics_create_circle_collider(f32 radius);
Constraint physics_create_constraint(Constraint_Type type, i32 length);


void update_position(Verlet_Object* obj, f32 dt);
void accelerate(Verlet_Object* obj, Vector2 acceleration);
void physics_cirlce_collider_solve(Verlet_Object* obj1, Circle_Collider collider1, Verlet_Object* obj2, Circle_Collider collider2);
void physics_apply_gravity_system();
void physics_apply_constraints_system();
void physics_solve_collision_system();
void physics_update_positions_system(f32 dt);
