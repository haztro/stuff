#include "graphics/window.h"

#include <stdlib.h>
#include <stdio.h>

Window* WINDOW_Create(int width, int height)
{

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    Window* window = (Window *) malloc(sizeof(Window));

	window->glfw_window = glfwCreateWindow(width, height, "Game", NULL, NULL);

    if (window->glfw_window == NULL)
    {
        printf("Failed to create GLFW window.\n");
        WINDOW_Destroy(window);
        glfwTerminate();
        return NULL;
    }

    return window;
}

void WINDOW_Destroy(Window *window)
{
	glfwDestroyWindow(window->glfw_window);
}