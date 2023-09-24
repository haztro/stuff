#ifndef RENDERER_H
#define RENDERER_H

#include <stdlib.h>
#include <cglm/cglm.h>
#include <glad/glad.h>

#include "graphics/vertex.h"
#include "graphics/texture.h"
#include "data/list.h"


typedef struct
{
	GLuint texture;
	Vertex top_left;
	Vertex top_right;
	Vertex bottom_left;
	Vertex bottom_right;
} Sprite;

typedef struct
{
	GLuint offset;
	GLuint num_vertices;
	GLuint texture;
} RenderBatch;

typedef struct
{
	GLuint vao;
	GLuint vbo;
	RenderBatch *render_batches;
	int num_batches;
	List *sprites;
	int num_vertices;
} Renderer;

Renderer* RENDERER_Create(int type);

void RENDERER_SpritesBegin(Renderer *renderer);
void RENDERER_SpritesRender(Renderer *renderer);
void RENDERER_SpritesEnd(Renderer *renderer);
void RENDERER_DrawSprite(Renderer *renderer, vec4 dest_rect, vec4 uv_rect, GLuint texture_id, Colour colour);

void RENDERER_LinesBegin(Renderer *renderer);
void RENDERER_LinesRender(Renderer *renderer);
void RENDERER_LinesEnd(Renderer *renderer);
void RENDERER_DrawLine(Renderer *renderer, vec2 pos1, vec2 pos2, Colour colour);
void RENDERER_DrawCircle(Renderer *renderer, vec2 pos, int radius, Colour colour);
void RENDERER_DrawRect(Renderer *renderer, vec4 dest_rect, Colour colour);

#endif