#include "graphics/ray.h"

#include <math.h>

static Colour white = {255, 255, 0, 255};

float distance_to_circle(vec2 pos, vec2 circle_pos, float radius);

RayCaster* RAY_Create()
{
	RayCaster *ray_caster = (RayCaster *) malloc(sizeof(RayCaster));
	ray_caster->objects = LIST_Create(32);

	return ray_caster;
}

void RAY_March(RayCaster *ray_caster, Renderer *renderer, vec2 pos1, vec2 dir)
{
	vec2 pos = {pos1[0], pos1[1]};
	
	float min_distance = RAY_GetMinDist(ray_caster, pos);

	int num = 0;

	while (min_distance > 2 && num <= 100)
	{
		
		pos[0] += min_distance * dir[0];
		pos[1] += min_distance * dir[1];

		if (pos[0] > 640 || pos[0] < 0 || pos[1] > 480 || pos[0] < 0)
			break;

		min_distance = RAY_GetMinDist(ray_caster, pos);
		num++;
	}

	RENDERER_DrawLine(renderer, pos1, pos, white);
}

float RAY_GetMinDist(RayCaster *ray_caster, vec2 pos)
{
	float min_distance = 0xFFFFFF;

	for (int i = 0; i < ray_caster->objects->used; i++)
	{
		Object *object = (Object *) ray_caster->objects->data[i];

		if (object->type == 0)
		{
			float dist = distance_to_circle(pos, object->pos, object->radius);
			if (dist < min_distance)
			{
				min_distance = dist;
			}
		}
	}

	return min_distance;
}

void RAY_AddCircle(RayCaster *ray_caster, vec2 pos, float radius)
{
	Object *object = (Object *) malloc(sizeof(Object));

	object->pos[0] = pos[0];
	object->pos[1] = pos[1];
	object->radius = radius;
	Colour colour = {255, 255, 255, 255};
	object->colour = colour;
	object->type = 0;

	LIST_PushBack(ray_caster->objects, object);
}

void RAY_Draw(RayCaster *ray_caster, Renderer *renderer)
{
	for (int i = 0; i < ray_caster->objects->used; i++)
	{
		Object *object = (Object *) ray_caster->objects->data[i];

		if (object->type == 0)
		{
			RENDERER_DrawCircle(renderer, object->pos, object->radius, object->colour);
		}
		else if (object->type == 1)
		{
			RENDERER_DrawRect(renderer, object->dest_rect, object->colour);
		}
	}
}

float distance_to_circle(vec2 pos, vec2 circle_pos, float radius)
{
	float x_dist = pos[0] - circle_pos[0];
	float y_dist = pos[1] - circle_pos[1];
	return sqrt(x_dist * x_dist + y_dist * y_dist) - radius;
}