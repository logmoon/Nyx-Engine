# include <math.h>
# include <stdio.h>
# include "physics.h"
# include "../ecs/ecs.h"
# include "../base_components/base_components.h"

Rigidbody physics_create_rigidbody(f32 starting_x_pos, f32 starting_y_pos, f32 gravity_scale)
{
	Vector2 initial = { .x = starting_x_pos, .y = starting_y_pos };
	Verlet_Object obj = { .current_position = initial, .previous_position = initial, .acceleration = { .x = 0, .y = 0 } };
	Rigidbody rb = { .obj = obj, .gravity_scale = gravity_scale };

	return rb;
}

Circle_Collider physics_create_circle_collider(f32 radius)
{
	Circle_Collider c = { .radius = radius };
	return c;
}

Constraint physics_create_constraint(Constraint_Type type, i32 length)
{
	Constraint con = { .type = type, .len = length };
	return con;
}

void update_position(Verlet_Object* obj, f32 dt)
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

void accelerate(Verlet_Object* obj, Vector2 acceleration)
{
	obj->acceleration.x += acceleration.x;
	obj->acceleration.y += acceleration.y;
}

void physics_cirlce_collider_solve(Verlet_Object* obj1, Circle_Collider collider1, Verlet_Object* obj2, Circle_Collider collider2)
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

void physics_apply_constraints_system()
{
	u32 i, j;
	Ecs_Query_Result* obj_qr = ecs_query(2, RIGIDBODY_COMPONENT, CIRCLE_COLLIDER_COMPONENT);

	for (i = 0; i < obj_qr->count; i++)
	{
		Rigidbody* rb = (Rigidbody*)ecs_get_component(obj_qr->entities[i], RIGIDBODY_COMPONENT);
		Circle_Collider* c = (Circle_Collider*)ecs_get_component(obj_qr->entities[i], CIRCLE_COLLIDER_COMPONENT);

		const f32 x = rb->obj.current_position.x;
		const f32 y = rb->obj.current_position.y;

		Ecs_Query_Result* con_qr = ecs_query(2, POSITION_COMPONENT, CONSTRAINT_COMPONENT);

		for (j = 0; j < con_qr->count; j++)
		{
			Position* pos = (Position*)ecs_get_component(con_qr->entities[j], POSITION_COMPONENT);
			Constraint* con = (Constraint*)ecs_get_component(con_qr->entities[j], CONSTRAINT_COMPONENT);

			switch (con->type)
			{
				case horizontal:
					if (x > pos->x && x < (pos->x + con->len) && y < pos->y)
					{
						rb->obj.current_position.y = pos->y - c->radius;
					}
					break;
				case vertical:
					if (pos->x >= 0)
					{
						if (y > (pos->y - c->radius - 0.1) && y < (pos->y + con->len - c->radius - 0.1) && x > pos->x)
						{
							if (rb->obj.current_position.x - rb->obj.previous_position.x > 0)
							{
								rb->obj.current_position.x = pos->x - c->radius;
							}
							else if (rb->obj.current_position.x - rb->obj.previous_position.x < 0)
							{
								rb->obj.current_position.x = pos->x + c->radius;
							}
						}
					}
					else
					{
						if (y > (pos->y - c->radius - 0.1) && y < (pos->y + con->len - c->radius - 0.1) && x < pos->x)
						{
							if (rb->obj.current_position.x - rb->obj.previous_position.x > 0)
							{
								rb->obj.current_position.x = pos->x - c->radius;
							}
							else if (rb->obj.current_position.x - rb->obj.previous_position.x < 0)
							{
								rb->obj.current_position.x = pos->x + c->radius;
							}
						}
					}
					break;
				default:
					return;
			}
		}
	}
}

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
