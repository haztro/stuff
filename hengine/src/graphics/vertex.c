#include "graphics/vertex.h"

void VERTEX_SetPosition(Vertex *vertex, float x, float y)
{
	vertex->position.x = x;
	vertex->position.y = y;
}

void VERTEX_SetColour(Vertex *vertex, GLubyte r, GLubyte g, GLubyte b, GLubyte a)
{
	vertex->colour.r = r;
	vertex->colour.g = g;
	vertex->colour.b = b;
	vertex->colour.a = a;
}

void VERTEX_SetUV(Vertex *vertex, float u, float v)
{
	vertex->uv.u = u;
	vertex->uv.v = v;
}
