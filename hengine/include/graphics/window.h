#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef struct Window_t
{
	GLFWwindow *glfw_window;
} Window;

Window* WINDOW_Create(int width, int height);
void WINDOW_Destroy(Window *window);


#endif