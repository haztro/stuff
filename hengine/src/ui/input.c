#include "ui/input.h"

#include <stdlib.h>
#include <stdio.h>

void INPUT_Init(InputManager *input_manager, GLFWwindow* window);
void INPUT_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void INPUT_MouseCallback(GLFWwindow* window, double xpos, double ypos);
void INPUT_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);


static InputManager *input_manager = {0}; 

InputManager* INPUT_Create(GLFWwindow* window)
{
	input_manager = (InputManager *) malloc(sizeof(InputManager));
	INPUT_Init(input_manager, window);

	return input_manager;
}

void INPUT_Init(InputManager *input_manager, GLFWwindow* window)
{
    // Set key callback.
    glfwSetKeyCallback(window, INPUT_KeyCallback);
    glfwSetCursorPosCallback(window, INPUT_MouseCallback);
    glfwSetMouseButtonCallback(window, INPUT_MouseButtonCallback);

    for (int i = 0; i < 348; i++)
    {
    	input_manager->keys[i] = 0;
    	input_manager->keys_prev[i] = 0;
    }

    input_manager->mouse_down = false;
}

void INPUT_Update(InputManager *input_manager)
{
	// Need to convert to map. 
	for (size_t i = 0; i < 348; i++)
	{
		input_manager->keys_prev[i] = input_manager->keys[i];
	}
}

bool INPUT_IsKeyDown(InputManager *input_manager, int key)
{
	if (input_manager->keys[key] != GLFW_RELEASE)
	{
		return true;
	}

	return false;
}

bool INPUT_IsKeyPressed(InputManager *input_manager, int key)
{
	if (input_manager->keys[key] == GLFW_PRESS &&
		input_manager->keys_prev[key] != GLFW_PRESS)
	{
		return true;
	}

	return false;
}

void INPUT_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key != -1)
	{	
		input_manager->keys[key] = action;
	}	
} 

void INPUT_MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	input_manager->mouse_pos[0] = xpos;
	input_manager->mouse_pos[1] = ypos;
}

void INPUT_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		input_manager->mouse_down = action;
	}
}