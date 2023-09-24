#ifndef SHADER_H
#define SHADER_H

typedef struct Shader_t
{
	int program;
	int vertex_shader;
	int fragment_shader;
} Shader;

Shader* SHADER_Create(const char *vert_path, const char *frag_path);

#endif