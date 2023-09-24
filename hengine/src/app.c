#include "app.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>


void APP_Init(App *app)
{

    glfwInit();

    // Create and Initialise window.
    app->window = WINDOW_Create(640, 480);

    // Create and Initialise input manager
    app->input_manager = INPUT_Create(app->window->glfw_window);

    glfwMakeContextCurrent(app->window->glfw_window);

    // Load glad.
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD.");
        app->is_running = false;
    }    

    // Set app to run. 
    app->is_running = true;

}

void APP_Run(App *app)
{

	while (app->is_running && !glfwWindowShouldClose(app->window->glfw_window))
	{
		app->Update();
		app->Draw();
        
        INPUT_Update(app->input_manager);

        glfwSwapBuffers(app->window->glfw_window);
        glfwPollEvents();
	}

	glfwTerminate();
}