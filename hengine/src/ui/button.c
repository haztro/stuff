#include "ui/button.h"

#include <stdio.h>


enum ClickState
{
	MOUSE_OFF,
	MOUSE_ON_DOWN,
	MOUSE_ON_UP,
	MOUSE_READY,
	MOUSE_INIT,
};

static bool BUTTON_IsMouseOver(Button *button, int mouse_x, int mouse_y);
static void BUTTON_ChangeState(UIManager *ui, Button *button, int state, int texture);

Button* BUTTON_Create(vec4 rect, void (*on_click)(void))
{
	Button *button = (Button *) malloc(sizeof(Button));

	glm_vec4_copy(rect, button->rect);
	glm_vec4_copy(GLM_VEC4_ZERO, button->uv);
	button->OnClick = on_click;
	button->mouse_on = false;
	button->state = MOUSE_INIT;

	return button;
}

void BUTTON_Update(UIManager *ui, Button *button, InputManager *input_manager)
{
	int mouse_x = (int) input_manager->mouse_pos[0];
	int mouse_y = 480 - (int) input_manager->mouse_pos[1];

	switch (button->state)
	{
		case MOUSE_OFF:

			if (BUTTON_IsMouseOver(button, mouse_x, mouse_y) && input_manager->mouse_down)
				BUTTON_ChangeState(ui, button, MOUSE_ON_DOWN, 1);
			else if (BUTTON_IsMouseOver(button, mouse_x, mouse_y) && !input_manager->mouse_down)
				BUTTON_ChangeState(ui, button, MOUSE_ON_UP, 1);
			break;

		case MOUSE_ON_DOWN:

			if (BUTTON_IsMouseOver(button, mouse_x, mouse_y) && !input_manager->mouse_down)
				BUTTON_ChangeState(ui, button, MOUSE_ON_UP, 1);
			else if (!BUTTON_IsMouseOver(button, mouse_x, mouse_y))
				BUTTON_ChangeState(ui, button, MOUSE_OFF, 0);

			break;

		case MOUSE_ON_UP:

			if (BUTTON_IsMouseOver(button, mouse_x, mouse_y) && input_manager->mouse_down)
				BUTTON_ChangeState(ui, button, MOUSE_READY, 2);
			else if (!BUTTON_IsMouseOver(button, mouse_x, mouse_y))
				BUTTON_ChangeState(ui, button, MOUSE_OFF, 0);
			break;

		case MOUSE_READY:

			if (BUTTON_IsMouseOver(button, mouse_x, mouse_y) && !input_manager->mouse_down)
			{
				button->OnClick();
				BUTTON_ChangeState(ui, button, MOUSE_ON_UP, 1);
			}
			else if (!BUTTON_IsMouseOver(button, mouse_x, mouse_y) && !input_manager->mouse_down)
			{
				BUTTON_ChangeState(ui, button, MOUSE_OFF, 0);
			}
			break;

		case MOUSE_INIT:
			BUTTON_ChangeState(ui, button, MOUSE_OFF, 0);
			break;
	}
}

static void BUTTON_ChangeState(UIManager *ui, Button *button, int state, int texture)
{
	TILESHEET_MakeUV(ui->tilesheet, button->uv, texture);
	button->state = state;
}

static bool BUTTON_IsMouseOver(Button *button, int mouse_x, int mouse_y)
{
	return (mouse_x > button->rect[0] && 
		mouse_x < button->rect[0] + button->rect[2] &&
		mouse_y > button->rect[1] && 
		mouse_y < button->rect[1] + button->rect[3]);
}

void BUTTON_Draw(UIManager *ui, Button *button)
{
	Colour colour = {255, 255, 255, 255};
	RENDERER_DrawSprite(ui->renderer, button->rect, button->uv, ui->tilesheet->texture->id, colour);
}