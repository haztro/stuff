#ifndef VERTEX_H
#define VERTEX_H

#include <glad/glad.h>

typedef struct Position_t
{
	float x;
	float y;
} Position;

typedef struct Colour_t
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} Colour;

typedef struct UV_t
{
	float u;
	float v;
} UV;

typedef struct Vertex_t
{
	Position position;
	Colour colour;
	UV uv;
} Vertex;

void VERTEX_SetPosition(Vertex *vertex, float x, float y);
void VERTEX_SetColour(Vertex *vertex, GLubyte r, uint8_t g, GLubyte b, GLubyte a);
void VERTEX_SetUV(Vertex *vertex, float u, float v);

#endif