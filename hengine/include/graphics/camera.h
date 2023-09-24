#ifndef CAMERA_H
#define CAMERA_H

#include <stdbool.h>
#include <cglm/cglm.h>

typedef struct
{
	int window_width;
	int window_height;
	mat4 camera_matrix;
	mat4 ortho_matrix;
	vec2 position;
	bool needs_update;
} Camera;

Camera* CAMERA_Create(int window_width, int window_height);


#endif