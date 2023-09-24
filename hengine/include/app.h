#ifndef APP_H
#define APP_H

#include "graphics/window.h"
#include "ui/input.h"


#include <stdbool.h>

typedef struct App_t
{
	Window *window;
	InputManager *input_manager;
	bool is_running;
	void (*Update)();
	void (*Draw)();
} App;

void APP_Init(App *app);
void APP_Run(App *app);

// TODO: make 'set callback' functions for update, draw.


#endif