#include "graphics/shader.h"

#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>

static int SHADER_InitShader(Shader *shader, const char *vert_path, 
                                const char *frag_path);
static int SHADER_CompileShader(int shader_id, const char *shader_path);
static int SHADER_LinkShaders(Shader *shader);

/******************************************************************************
 * NAME: SHADER_Create
 *
 * DESCRIPTION: Allocate memory for the new shader and initialise it.  
 *
 * PARAMETERS:  Source code for vertex shader 
 *              Source code for fragment shader
 *
 * RETURNS: Pointer to the new shader object
 *****************************************************************************/
Shader* SHADER_Create(const char *vert_path, const char *frag_path)
{
    Shader* shader = (Shader*) malloc(sizeof(Shader));
    SHADER_InitShader(shader, vert_path, frag_path);

    return shader;
}

/******************************************************************************
 * NAME: SHADER_InitShader
 *
 * DESCRIPTION: Create GL instance of shader and compile vertex and fragment
 *              using given source files.  
 *
 * PARAMETERS:  Pointer to shader that is being initialised
 *              Source code for vertex shader 
 *              Source code for fragment shader 
 *
 * RETURNS: int
 *****************************************************************************/
static int SHADER_InitShader(Shader *shader, const char *vert_path, 
                                const char *frag_path)
{
	shader->vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	shader->fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	SHADER_CompileShader(shader->vertex_shader, vert_path);
	SHADER_CompileShader(shader->fragment_shader, frag_path);

	SHADER_LinkShaders(shader);

	return 0;
}

/******************************************************************************
 * NAME: SHADER_CompileShader
 *
 * DESCRIPTION: Compile the glsl shader.   
 *
 * PARAMETERS:  shader_id
 *              Source code for shader 
 *
 * RETURNS: -1 if failed to create, 0 otherwise. 
 *****************************************************************************/
static int SHADER_CompileShader(int shader_id, const char *shader_path)
{

    char *shader_source;
    int length = 0;
    FILE *file = fopen(shader_path, "r");

    if (file == NULL)
    {
        printf("File %s does not exist.\n", shader_path);
        return -1;
    }

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    shader_source = malloc(length + 1);

    if (shader_source)
    {
        fread(shader_source, 1, length, file);
    }
    fclose(file);

    shader_source[length] = '\0';

	glShaderSource(shader_id, 1, (const GLchar * const*) &shader_source, NULL);
	glCompileShader(shader_id);

	int success;
	char info_log[512];

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader_id, 512, NULL, info_log);
        printf("Shader Compilation Error: %s\n", info_log);
        return -1;
    }

    return 0;
}

/******************************************************************************
 * NAME: SHADER_LinkShaders
 *
 * DESCRIPTION: Attach vertex, fragment shader to program and link them.  
 *
 * PARAMETERS: shader
 *
 * RETURNS: -1 if failed to link, 0 otherwise. 
 *****************************************************************************/
static int SHADER_LinkShaders(Shader *shader)
{
	shader->program = glCreateProgram();
    glAttachShader(shader->program, shader->vertex_shader);
    glAttachShader(shader->program, shader->fragment_shader);
    glLinkProgram(shader->program);

    int success;
    char info_log[512];

    glGetProgramiv(shader->program, GL_LINK_STATUS, &success);
    if (!success) 
    {
        glGetProgramInfoLog(shader->program, 512, NULL, info_log);
        printf("Shader linking failed: %s\n", info_log);
		return -1;
    }

    glDeleteShader(shader->vertex_shader);
    glDeleteShader(shader->fragment_shader);

    return 0;
}