#include "app.h"
#include "graphics/window.h"
#include "graphics/renderer.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/tilesheet.h"
#include "graphics/camera.h"
#include "data/list.h"
#include "ui/ui.h"

#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// void processInput(GLFWwindow *window);
void draw();
void update();
void click_test();

App app;
Shader *shader;
Renderer *renderer;
Shader *line_shader;
Renderer *line_renderer;
vec4 dest_rect = {100, 100, 7 * 4, 8 * 4};
vec4 uv_rect;
Colour colour = {255, 255, 255, 255};
Tilesheet *tilesheet;
Camera *camera;
UIManager *ui;
Texture *ui_texture;
Tilesheet *ui_tilesheet;

int main()
{

    APP_Init(&app);

    shader = SHADER_Create("assets/vert_shader.vert", "assets/frag_shader.frag");
    renderer = RENDERER_Create(0);

    line_shader = SHADER_Create("assets/vert_line_shader.vert", "assets/frag_line_shader.frag");
    line_renderer = RENDERER_Create(1);
    tilesheet = TILESHEET_Create("font.png", 9, 3);
    camera = CAMERA_Create(640, 480);

    ui = UI_Create();
    vec4 brect = {200, 200, 64, 32};
    vec4 brect1 = {200, 300, 64, 32};
    UI_AddButton(ui, brect, click_test);
    UI_AddButton(ui, brect1, click_test);

    app.Draw = draw;
    app.Update = update;

    APP_Run(&app);

    return 0;
}

void click_test()
{
    static int num = 0;
    num++;
    printf("CLICK: %d\n", num);
}

void draw() 
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // // Draw Game stuff
    glActiveTexture(GL_TEXTURE0);

    glUseProgram(shader->program);
    GLint loc = glGetUniformLocation(shader->program, "sampler");
    glUniform1i(loc, 0);

    loc = glGetUniformLocation(shader->program, "P");
    glUniformMatrix4fv(loc, 1, GL_FALSE, &camera->ortho_matrix[0][0]);

    RENDERER_SpritesBegin(renderer);

    TILESHEET_MakeUV(tilesheet, uv_rect, 0);
    RENDERER_DrawSprite(renderer, dest_rect, uv_rect, tilesheet->texture->id, colour);
    RENDERER_SpritesRender(renderer);

    RENDERER_SpritesEnd(renderer);
    glUseProgram(0);

    loc = glGetUniformLocation(shader->program, "P");
    glUniformMatrix4fv(loc, 1, GL_FALSE, &camera->ortho_matrix[0][0]);

//////////////////////////////////////


    glUseProgram(line_shader->program);

    GLuint loc1 = glGetUniformLocation(line_shader->program, "P");
    glUniformMatrix4fv(loc1, 1, GL_FALSE, &camera->ortho_matrix[0][0]);

    RENDERER_LinesBegin(line_renderer);

    vec2 pos1 = {100, 100};
    vec2 pos2 = {200, 200};

    RENDERER_DrawLine(line_renderer, pos1, pos2, colour);
    RENDERER_LinesRender(line_renderer);

    RENDERER_LinesEnd(line_renderer);
    glUseProgram(0);


    // Draw UI
    UI_Draw(ui);


}

void update()
{
    UI_Update(ui, app.input_manager);

    if (INPUT_IsKeyDown(app.input_manager, GLFW_KEY_W))
    {
        dest_rect[1] += 10;
    }
    if (INPUT_IsKeyDown(app.input_manager, GLFW_KEY_S))
    {
        dest_rect[1] -= 10;
    }

    if (INPUT_IsKeyDown(app.input_manager, GLFW_KEY_A))
    {
        dest_rect[0] -= 10;
    }
    if (INPUT_IsKeyDown(app.input_manager, GLFW_KEY_D))
    {
        dest_rect[0] += 10;
    }

    if (INPUT_IsKeyPressed(app.input_manager, GLFW_KEY_Y))
    {
        printf("PRESSED\n");
    }
    if (INPUT_IsKeyDown(app.input_manager, GLFW_KEY_Y))
    {
        printf("DOWN\n");
    }

}

