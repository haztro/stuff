#ifndef UI_H
#define UI_H

#include "graphics/renderer.h"
#include "graphics/tilesheet.h"
#include "graphics/camera.h"
#include "graphics/shader.h"
#include "ui/input.h"

#include <cglm/cglm.h>

typedef struct
{
	Renderer *renderer;
	Shader *shader;
	Camera *viewport;
	Tilesheet *tilesheet;
	List *buttons;
	
} UIManager;

UIManager* UI_Create();
void UI_Update(UIManager *ui, InputManager *input_manager);
void UI_Draw(UIManager *ui);
void UI_AddButton(UIManager *ui, vec4 dest, void (*on_click)(void));

#endif