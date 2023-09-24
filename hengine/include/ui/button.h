#ifndef BUTTON_H
#define BUTTON_H

#include "ui/ui.h"
#include "ui/input.h"
#include "graphics/renderer.h"
#include "graphics/tilesheet.h"

#include <cglm/cglm.h>

typedef struct
{
	vec4 rect;
	vec4 uv;
	bool mouse_on;
	int state;
	void (*OnClick)(void);
} Button;

Button* BUTTON_Create(vec4 rect, void (*on_click)(void));
void BUTTON_Update(UIManager *ui, Button *button, InputManager *input_manager);
void BUTTON_Draw(UIManager *ui, Button *button);

#endif