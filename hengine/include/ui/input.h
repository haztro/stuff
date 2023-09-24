#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

typedef struct InputManager InputManager;
struct InputManager
{
	int keys[348]; // Number of different keys
	int keys_prev[348];
	vec2 mouse_pos;
	bool mouse_down;
};

InputManager* INPUT_Create(GLFWwindow* window);
bool INPUT_IsKeyDown(InputManager *input_manager, int key);
bool INPUT_IsKeyPressed(InputManager *input_manager, int key);
void INPUT_Update(InputManager *input_manager);


#endif