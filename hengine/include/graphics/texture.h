#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

typedef struct Texture_t
{
	GLuint id;
	int width;
	int height;
} Texture;

Texture* TEXTURE_Create(const char *filename);

#endif 