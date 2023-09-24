#include "ui/ui.h"
#include "data/list.h"

#include "ui/button.h"

#include <stdlib.h>


UIManager* UI_Create()
{
	UIManager* ui = (UIManager *) malloc(sizeof(UIManager));

	ui->buttons = LIST_Create(32);
	ui->renderer = RENDERER_Create(0);
	ui->shader = SHADER_Create("assets/vert_shader.vert", "assets/frag_shader.frag");
	ui->viewport = CAMERA_Create(640, 480);
	ui->tilesheet = TILESHEET_Create("ui.png", 3, 1);

	return ui;
}

void UI_AddButton(UIManager *ui, vec4 dest, void (*on_click)(void))
{
	Button *button = BUTTON_Create(dest, on_click);
	LIST_PushBack(ui->buttons, (void *) button);
}

void UI_Update(UIManager *ui, InputManager *input_manager)
{
	for (int i = 0; i < ui->buttons->used; i++)
	{
		Button *button = (Button *) ui->buttons->data[i];
		BUTTON_Update(ui, button, input_manager);
	}
}

void UI_Draw(UIManager *ui)
{

    glUseProgram(ui->shader->program);
    GLint loc = glGetUniformLocation(ui->shader->program, "sampler");
    glUniform1i(loc, 0);

    loc = glGetUniformLocation(ui->shader->program, "P");
    glUniformMatrix4fv(loc, 1, GL_FALSE, &ui->viewport->ortho_matrix[0][0]);

    RENDERER_SpritesBegin(ui->renderer);

	// Draw buttons
	for (int i = 0; i < ui->buttons->used; i++)
	{
		Button *button = (Button *) ui->buttons->data[i];
		BUTTON_Draw(ui, button);	
	}

    RENDERER_SpritesRender(ui->renderer);
    RENDERER_SpritesEnd(ui->renderer);
    glUseProgram(0);

}
