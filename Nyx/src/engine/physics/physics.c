# include <math.h>
# include "physics.h"
# include "../ecs/ecs.h"
# include "../base_components/base_components.h"

Rigidbody physics_create_rigidbody(f32 gravity_scale)
{
	Vector2 zero = { .x = 0, .y = 0 };
	Velet_Object obj = { .current_position = zero, .previous_position = zero, .acceleration = zero };
	Rigidbody rb = { .obj = obj, .gravity_scale = gravity_scale };

	return rb;
}

Circle_Collider physics_create_circle_collider(f32 radius)
{
	Circle_Collider c = { .radius = radius };
	return c;
}

void update_position(Velet_Object* obj, f32 dt)
{
	// Calculate the velocity
	f32 x = obj->current_position.x - obj->previous_position.x;
	f32 y = obj->current_position.y - obj->previous_position.y;
	const Vector2 velocity = { .x = x, .y = y };

	// Update position
	obj->previous_position = obj->current_position;

	// Perform velet integration
	x = obj->current_position.x + velocity.x + obj->acceleration.x * dt * dt;
	y = obj->current_position.y + velocity.y + obj->acceleration.y * dt * dt;
	obj->current_position.x = x;
	obj->current_position.y = y;

	// Reset acceleration
	obj->acceleration.x = 0;
	obj->acceleration.y = 0;
}

void accelerate(Velet_Object* obj, Vector2 acceleration)
{
	obj->acceleration.x += acceleration.x;
	obj->acceleration.y += acceleration.y;
}

void physics_cirlce_collider_solve(Velet_Object* obj1, Circle_Collider collider1, Velet_Object* obj2, Circle_Collider collider2)
{
	const Vector2 collisiton_axis = { .x = obj1->current_position.x - obj2->current_position.x,
		.y = obj1->current_position.y - obj2->current_position.y };

	const f32 dist = sqrt((collisiton_axis.x * collisiton_axis.x) + (collisiton_axis.y * collisiton_axis.y));
	const f32 min_dist = collider1.radius + collider2.radius;
	if (dist < min_dist)
	{
		const Vector2 n = { .x = collisiton_axis.x / dist, .y = collisiton_axis.y / dist };
		const f32 delta = min_dist - dist;

		obj1->current_position.x += 0.5 * delta * n.x;
		obj1->current_position.y += 0.5 * delta * n.y;

		obj2->current_position.x -= 0.5 * delta * n.x;
		obj2->current_position.y -= 0.5 * delta * n.y;
	}
}

void physics_apply_gravity_system()
{
	Ecs_Query_Result* qr = ecs_query(1, RIGIDBODY_COMPONENT);
	for (u32 i = 0; i < qr->count; i++)
	{
		Rigidbody* rb = (Rigidbody*)ecs_get_component(qr->entities[i],
				RIGIDBODY_COMPONENT);

		// Applying gravity
		Vector2 gravity = { .x = 0, .y = GRAVITY * rb->gravity_scale };
		accelerate(&rb->obj, gravity);
	}
}

/*
void physics_apply_constraints_system(f32 dt)
{
	Ecs_Query_Result* qr = ecs_query(3, CIRCLE_COLLIDER_COMPONENT, RIGIDBODY_COMPONENT, CONSTRAINT_COMPONENT);
	for (u32 i = 0; i < qr->count; i++)
	{
		Circle_Collider* c = (Circle_Collider*)ecs_get_component(qr->entities[i],
				CIRCLE_COLLIDER_COMPONENT);
		Rigidbody* rb = (Rigidbody*)ecs_get_component(qr->entities[i],
				RIGIDBODY_COMPONENT);

		// Solving constraints
		for (u32 k = 0; k < qr->count; k++)
		{
			// TODO Figure out how to do this
			Constraint* con = (Constraint*)ecs_get_component(qr->entities[k], CONSTRAINT_COMPONENT);
		}
	}
}
*/

void physics_solve_collision_system()
{
	Ecs_Query_Result* qr = ecs_query(2, CIRCLE_COLLIDER_COMPONENT, RIGIDBODY_COMPONENT);
	for (u32 i = 0; i < qr->count; i++)
	{
		Circle_Collider* c = (Circle_Collider*)ecs_get_component(qr->entities[i],
				CIRCLE_COLLIDER_COMPONENT);
		Rigidbody* rb = (Rigidbody*)ecs_get_component(qr->entities[i],
				RIGIDBODY_COMPONENT);

		// Solving collisions
		for (u32 k = i+1; k < qr->count; k++)
		{
			Circle_Collider* c2 = (Circle_Collider*)ecs_get_component(qr->entities[k],
					CIRCLE_COLLIDER_COMPONENT);
			Rigidbody* rb2 = (Rigidbody*)ecs_get_component(qr->entities[k],
					RIGIDBODY_COMPONENT);

			physics_cirlce_collider_solve(&rb->obj, *c, &rb2->obj, *c2);
		}
	}
}

void physics_update_positions_system(f32 dt)
{
	Ecs_Query_Result* qr = ecs_query(2, POSITION_COMPONENT, RIGIDBODY_COMPONENT);
	for (u32 i = 0; i < qr->count; i++)
	{
		Rigidbody* rb = (Rigidbody*)ecs_get_component(qr->entities[i],
				RIGIDBODY_COMPONENT);
		Position* position = (Position*)ecs_get_component(qr->entities[i], POSITION_COMPONENT);

		// Updating position
		update_position(&rb->obj, dt);
		position->x = rb->obj.current_position.x;
		position->y = rb->obj.current_position.y;
	}
}
