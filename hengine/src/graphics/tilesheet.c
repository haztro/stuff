#include "graphics/tilesheet.h"

void TILESHEET_Init(Tilesheet *tilesheet, Texture *texture, int dim_x, int dim_y);


Tilesheet* TILESHEET_Create(const char *image_path, int dim_x, int dim_y)
{
	Tilesheet *tilesheet = (Tilesheet *) malloc(sizeof(Tilesheet));

	Texture *texture = TEXTURE_Create(image_path);
	TILESHEET_Init(tilesheet, texture, dim_x, dim_y);

	return tilesheet;
}

void TILESHEET_Init(Tilesheet *tilesheet, Texture *texture, int dim_x, int dim_y)
{
	tilesheet->texture = texture;
	tilesheet->dim_x = dim_x;
	tilesheet->dim_y = dim_y;
}

void TILESHEET_MakeUV(Tilesheet *tilesheet, vec4 dest_uv, int index)
{
	int x_tile = index % tilesheet->dim_x;
	int y_tile = index / tilesheet->dim_y;

	dest_uv[0] = x_tile / (float) tilesheet->dim_x; 
	dest_uv[1] = y_tile / (float) tilesheet->dim_y;
	dest_uv[2] = 1.0f / tilesheet->dim_x;
	dest_uv[3] = 1.0f / tilesheet->dim_y;
}