#include "graphics/renderer.h"
#include <math.h>

#define CIRCLE_RES (40)

static void RENDERER_SpriteInit(Sprite *sprite, vec4 dest_rect, vec4 uv_rect, GLuint texture_id, Colour colour);
static void RENDERER_CreateSpriteBatch(Renderer *renderer);
static void RENDERER_SpriteVertexArray(Renderer *renderer);
static void RENDERER_LinesVertexArray(Renderer *renderer);

/******************************************************************************
 * NAME: RENDERER_Create
 *
 * DESCRIPTION: Allocate memory for the new renderer and initialise it.  
 *
 * PARAMETERS: void
 *
 * RETURNS: Pointer to the new renderer object
 *****************************************************************************/
Renderer* RENDERER_Create(int type)
{
	Renderer* renderer = (Renderer *) malloc(sizeof(Renderer));

	renderer->render_batches = NULL;
	renderer->num_batches = 0;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	if (!type)
	{
		RENDERER_SpriteVertexArray(renderer);
		renderer->sprites = LIST_Create(32);
	}
	else
	{
		RENDERER_LinesVertexArray(renderer);
		renderer->sprites = LIST_Create(32);
	}


	return renderer;
}

void RENDERER_SpritesBegin(Renderer *renderer)
{
	LIST_Clear(renderer->sprites);
}

void RENDERER_SpritesRender(Renderer *renderer)
{
	glBindVertexArray(renderer->vao);

	for (int i = 0; i < renderer->num_batches; i++)
	{
		glBindTexture(GL_TEXTURE_2D, renderer->render_batches[i].texture);
		glDrawArrays(GL_TRIANGLES, renderer->render_batches[i].offset, renderer->num_vertices);		
	}

	glBindVertexArray(0);
}

void RENDERER_SpritesEnd(Renderer *renderer)
{
	RENDERER_CreateSpriteBatch(renderer);
}

void RENDERER_DrawSprite(Renderer *renderer, vec4 dest_rect, vec4 uv_rect, 
							GLuint texture_id, Colour colour)
{
	Sprite *sprite = (Sprite *) malloc(sizeof(Sprite));	
	RENDERER_SpriteInit(sprite, dest_rect, uv_rect, texture_id, colour);	
	LIST_PushBack(renderer->sprites, (void *) sprite);
}

static void RENDERER_CreateSpriteBatch(Renderer *renderer)
{
	if (renderer->sprites->used == 0)
	{
		return;
	}

	Vertex *vertices = (Vertex *) malloc(renderer->sprites->used * sizeof(Vertex) * 6);

	free(renderer->render_batches);
	renderer->render_batches = (RenderBatch *) malloc(sizeof(RenderBatch));
	renderer->num_batches = 0;

	renderer->num_vertices = 0;
	size_t current_vertex = 0;
	size_t offset = 0;

	for (size_t cs = 0; cs < renderer->sprites->used; cs++)
	{
		// If it's the first batch or a new texture is detected
		// add another render batch. 

		Sprite *current_sprite = (Sprite *) renderer->sprites->data[cs];
		Sprite *last_sprite;

		if (cs > 0)	
			last_sprite = (Sprite *) renderer->sprites->data[cs - 1];

		if (renderer->num_batches == 0 || 
			current_sprite->texture != last_sprite->texture)
		{
			RenderBatch render_batch = (RenderBatch) {offset, 6, current_sprite->texture};
			renderer->render_batches[renderer->num_batches++] = render_batch;
			renderer->render_batches = (RenderBatch *) realloc(renderer->render_batches, 
																renderer->num_batches * sizeof(RenderBatch));
		}
		else
		{
			// Otherwise just increase its vertices. 
			renderer->render_batches[renderer->num_batches - 1].num_vertices += 6;
		}
		renderer->num_vertices += 6;
		vertices[current_vertex++] = current_sprite->top_left;
		vertices[current_vertex++] = current_sprite->bottom_left;
		vertices[current_vertex++] = current_sprite->bottom_right;
		vertices[current_vertex++] = current_sprite->bottom_right;
		vertices[current_vertex++] = current_sprite->top_right;
		vertices[current_vertex++] = current_sprite->top_left;
		offset += 6;
	}

	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, renderer->num_vertices * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->num_vertices * sizeof(Vertex), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	free(vertices);
}

static void RENDERER_SpriteVertexArray(Renderer *renderer)
{
	glGenVertexArrays(1, &renderer->vao);
	glBindVertexArray(renderer->vao);

	glGenBuffers(1, &renderer->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *) offsetof(Vertex, colour));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, uv));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

static void RENDERER_SpriteInit(Sprite *sprite, vec4 dest_rect, vec4 uv_rect, 
									GLuint texture_id, Colour colour)

{
	sprite->texture = texture_id;

	VERTEX_SetPosition(&sprite->top_left, dest_rect[0], dest_rect[1]);
	VERTEX_SetUV(&sprite->top_left, uv_rect[0], uv_rect[1]);
	sprite->top_left.colour = colour;

	VERTEX_SetPosition(&sprite->top_right, dest_rect[0] + dest_rect[2], 
						dest_rect[1]);
	VERTEX_SetUV(&sprite->top_right, uv_rect[0] + uv_rect[2], uv_rect[1]);
	sprite->top_right.colour = colour;

	VERTEX_SetPosition(&sprite->bottom_left, dest_rect[0], 
						dest_rect[1] + dest_rect[3]);
	VERTEX_SetUV(&sprite->bottom_left, uv_rect[0], uv_rect[1] + uv_rect[3]);
	sprite->bottom_left.colour = colour;

	VERTEX_SetPosition(&sprite->bottom_right, dest_rect[0] + dest_rect[2], 
						dest_rect[1] + dest_rect[3]);
	VERTEX_SetUV(&sprite->bottom_right, uv_rect[0] + uv_rect[2], uv_rect[1] + uv_rect[3]);
	sprite->bottom_right.colour = colour;
}

void RENDERER_LinesBegin(Renderer *renderer)
{
	LIST_Clear(renderer->sprites);
}

void RENDERER_LinesRender(Renderer *renderer)
{
	glBindVertexArray(renderer->vao);

	glDrawArrays(GL_LINES, 0, renderer->num_vertices);	

	glBindVertexArray(0);
}

void RENDERER_LinesEnd(Renderer *renderer)
{

	Vertex *vertices = (Vertex *) malloc(renderer->sprites->used * sizeof(Vertex));
	renderer->num_vertices = 0;

	for (int i = 0; i < renderer->sprites->used; i++)
	{
		Vertex *current_vertex = (Vertex *) renderer->sprites->data[i];
		vertices[i] = *current_vertex;
		renderer->num_vertices++;
	}

	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, renderer->num_vertices * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->num_vertices * sizeof(Vertex), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    free(vertices);
}

void RENDERER_DrawLine(Renderer *renderer, vec2 pos1, vec2 pos2, Colour colour)
{
	Vertex *vertex1 = (Vertex *) malloc(sizeof(Vertex));
	VERTEX_SetPosition(vertex1, pos1[0], pos1[1]);
	vertex1->colour = colour;

	Vertex *vertex2 = (Vertex *) malloc(sizeof(Vertex));
	VERTEX_SetPosition(vertex2, pos2[0], pos2[1]);
	vertex2->colour = colour;

	LIST_PushBack(renderer->sprites, (void *) vertex1);
	LIST_PushBack(renderer->sprites, (void *) vertex2);
}

void RENDERER_DrawCircle(Renderer *renderer, vec2 pos, int radius, Colour colour)
{
	vec2 pos1;
	pos1[0] = pos[0] + radius * cos(0);
	pos1[1] = pos[1] + radius * sin(0);

	for (int i = 1; i < CIRCLE_RES + 1; i++)
	{
		float angle = 2.0 * M_PI * i / CIRCLE_RES;

		vec2 pos2;
		pos2[0] = pos[0] + radius * cos(angle);
		pos2[1] = pos[1] + radius * sin(angle);

		RENDERER_DrawLine(renderer, pos1, pos2, colour);

		pos1[0] = pos2[0];
		pos1[1] = pos2[1];
	}
}

void RENDERER_DrawRect(Renderer *renderer, vec4 dest_rect, Colour colour)
{
	vec2 pos1 = {dest_rect[0], dest_rect[1]};
	vec2 pos2 = {dest_rect[0] + dest_rect[2], dest_rect[1]};
	vec2 pos3 = {dest_rect[0] + dest_rect[2], dest_rect[1] + dest_rect[3]};
	vec2 pos4 = {dest_rect[0], dest_rect[1] + dest_rect[3]};

	RENDERER_DrawLine(renderer, pos1, pos2, colour);
	RENDERER_DrawLine(renderer, pos2, pos3, colour);
	RENDERER_DrawLine(renderer, pos3, pos4, colour);
	RENDERER_DrawLine(renderer, pos4, pos1, colour);
}

static void RENDERER_LinesVertexArray(Renderer *renderer)
{
	glGenVertexArrays(1, &renderer->vao);
	glBindVertexArray(renderer->vao);

	glGenBuffers(1, &renderer->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *) offsetof(Vertex, colour));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}