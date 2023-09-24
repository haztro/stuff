#include "graphics/camera.h"

Camera* CAMERA_Create(int window_width, int window_height)
{
	Camera* camera = (Camera*) malloc(sizeof(Camera));

	camera->window_width = window_width;
	camera->window_height = window_height;

	camera->position[0] = 0;
	camera->position[1] = 0;

	glm_mat4_zero(camera->ortho_matrix);
	camera->ortho_matrix[0][0] = 2.0 / (float) window_width;
	camera->ortho_matrix[1][1] = 2.0 / (float) window_height;
	camera->ortho_matrix[2][2] = -1.0;
	camera->ortho_matrix[3][0] = -1.0;
	camera->ortho_matrix[3][1] = -1.0;
	camera->ortho_matrix[3][2] = -1.0;
	camera->ortho_matrix[3][3] = 1.0;

	glm_mat4_zero(camera->camera_matrix);

	return camera;
}