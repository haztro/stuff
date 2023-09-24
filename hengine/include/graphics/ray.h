#ifndef RAY_H
#define RAY_H

#include "data/list.h"
#include "graphics/renderer.h"
#include "graphics/vertex.h"
#include <cglm/cglm.h>


typedef struct
{
	vec4 dest_rect;
	vec2 pos;
	float radius;
	Colour colour;
	int type;
} Object;

typedef struct
{
	List *objects;
} RayCaster;

RayCaster* RAY_Create();
void RAY_Draw(RayCaster *ray_caster, Renderer *renderer);
void RAY_AddCircle(RayCaster *ray_caster, vec2 pos, float radius);
void RAY_March(RayCaster *ray_caster, Renderer *renderer, vec2 pos, vec2 dir);
float RAY_GetMinDist(RayCaster *ray_caster, vec2 pos);

#endif 