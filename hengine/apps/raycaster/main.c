#include "hengine.h"

void draw();
void update();

App app;
Shader *shader;
Renderer *renderer;
Colour red = {255, 0, 0, 255};
Colour white = {255, 255, 255, 255};
Camera *camera;
RayCaster *ray_caster;
vec2 player_pos = {100, 300};
vec2 look_dir = {0, 0};
vec2 final = {0, 0};
float min_distance = 0;

int main()
{

    APP_Init(&app);

    shader = SHADER_Create("assets/vert_line_shader.vert", "assets/frag_line_shader.frag");
    renderer = RENDERER_Create(1);
    camera = CAMERA_Create(640, 480);

    ray_caster = RAY_Create();
    vec2 pos = {100, 100};
    RAY_AddCircle(ray_caster, pos, 50);
    vec2 pos1 = {400, 200};
    RAY_AddCircle(ray_caster, pos1, 70);
    vec2 pos2 = {500, 400};
    RAY_AddCircle(ray_caster, pos2, 30);

    app.Draw = draw;
    app.Update = update;

    APP_Run(&app);

    return 0;
}

void draw() 
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader->program);

    GLuint loc1 = glGetUniformLocation(shader->program, "P");
    glUniformMatrix4fv(loc1, 1, GL_FALSE, &camera->ortho_matrix[0][0]);

    RENDERER_LinesBegin(renderer);

    // Draw Player

    // vec2 look_pos = {player_pos[0] + look_dir[0] * 100, player_pos[1] + look_dir[1] * 100};

    for (int i = 0; i < 300; i++)
    {
        float angle = 2 * M_PI * i / 300.0;
        look_dir[0] = cos(angle);
        look_dir[1] = sin(angle);
        RAY_March(ray_caster, renderer, player_pos, look_dir);
    }

    RENDERER_DrawCircle(renderer, player_pos, 5, red);

    // Draw World
    RAY_Draw(ray_caster, renderer);

    RENDERER_LinesRender(renderer);

    RENDERER_LinesEnd(renderer);
    glUseProgram(0);

}

void update()
{

    if (INPUT_IsKeyDown(app.input_manager, GLFW_KEY_W))
    {
        player_pos[1] += 10;
    }
    if (INPUT_IsKeyDown(app.input_manager, GLFW_KEY_S))
    {
        player_pos[1] -= 10;
    }

    if (INPUT_IsKeyDown(app.input_manager, GLFW_KEY_A))
    {
        player_pos[0] -= 10;
    }
    if (INPUT_IsKeyDown(app.input_manager, GLFW_KEY_D))
    {
        player_pos[0] += 10;
    }

    // look_dir[0] = -(player_pos[0] - app.input_manager->mouse_pos[0]);
    // look_dir[1] = -(player_pos[1] - (480 - app.input_manager->mouse_pos[1]));
    // float mag = sqrt(look_dir[0] * look_dir[0] + look_dir[1] * look_dir[1]);
    // look_dir[0] /= mag;
    // look_dir[1] /= mag;

}

