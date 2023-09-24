#ifndef TILESHEET_H
#define TILESHEET_H

#include "graphics/texture.h"

#include <cglm/cglm.h>

typedef struct Tilesheet Tilesheet;
struct Tilesheet
{
	Texture *texture;
	int dim_x;
	int dim_y;
};

Tilesheet* TILESHEET_Create(const char *image_path, int dim_x, int dim_y);

void TILESHEET_MakeUV(Tilesheet *tilesheet, vec4 dest_uv, int index);

#endif 