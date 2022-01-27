#include <main_state.h>
#include <glad/glad.h>
#include <math.h>


#include <rafgl.h>

#include <game_constants.h>

#define vert_count 48

typedef struct _vertex_t
{
    vec3_t position;
    vec3_t color;
    float alpha;
    float u, v;
    vec3_t normal;
} vertex_t;

vertex_t vertex(vec3_t pos, vec3_t  col, float alpha, float u, float v, vec3_t normal)
{
    vertex_t vert;

    vert.position = pos;
    vert.color = col;
    vert.alpha = alpha;
    vert.u = u;
    vert.v = v;
    vert.normal = normal;

    return vert;
}
vertex_t vertices[vert_count];

static GLuint vao, vbo;
static GLuint shader_program_id, uni_M, uni_VP, uni_light_color, uni_light_direction, uni_light_source;
static rafgl_raster_t raster;
static rafgl_texture_t texture_doog;
static rafgl_texture_t texture_floor;
static rafgl_texture_t texture_stone;

//svetlo
vec3_t light_color = RAFGL_WHITE;
vec3_t light_direction = vec3m(-1.0f, -1.0f, -1.0f);

void main_state_init(GLFWwindow *window, void *args, int width, int height)
{

    rafgl_raster_load_from_image(&raster, "res/images/doge.png");

    rafgl_texture_init(&texture_doog);
    rafgl_texture_load_from_raster(&texture_doog, &raster);

    glBindTexture(GL_TEXTURE_2D, texture_doog.tex_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    rafgl_raster_load_from_image(&raster, "res/images/floor.jpg");

    rafgl_texture_init(&texture_floor);
    rafgl_texture_load_from_raster(&texture_floor, &raster);

    glBindTexture(GL_TEXTURE_2D, texture_floor.tex_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    rafgl_raster_load_from_image(&raster, "res/images/stone.jpg");

    rafgl_texture_init(&texture_stone);
    rafgl_texture_load_from_raster(&texture_stone, &raster);

    glBindTexture(GL_TEXTURE_2D, texture_stone.tex_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    //Shiba 1
    vertices[0] = vertex(vec3(-1.0f, 1.0f, 3.0f), RAFGL_RED, 1.0f, 0.0f, 0.0f, RAFGL_VEC3_NEGZ);
    vertices[1] = vertex(vec3(-1.0f, -1.0f, 3.0f), RAFGL_GREEN, 1.0f, 0.0f, 1.0f, RAFGL_VEC3_NEGZ);
    vertices[2] = vertex(vec3(1.0f, 1.0f, 3.0f), RAFGL_GREEN, 1.0f, 1.0f, 0.0f, RAFGL_VEC3_NEGZ);

    vertices[3] = vertex(vec3(1.0f, 1.0f, 3.0f), RAFGL_GREEN, 1.0f, 1.0f, 0.0f, RAFGL_VEC3_NEGZ);
    vertices[4] = vertex(vec3(-1.0f, -1.0f, 3.0f), RAFGL_GREEN, 1.0f, 0.0f, 1.0f, RAFGL_VEC3_NEGZ);
    vertices[5] = vertex(vec3(1.0f, -1.0f, 3.0f), RAFGL_BLUE, 1.0f, 1.0f, 1.0f, RAFGL_VEC3_NEGZ);

    //Shiba 2
    vertices[6] = vertex(vec3(-1.0f, 1.0f, -1.0f), RAFGL_RED, 1.0f, 0.0f, 0.0f, RAFGL_VEC3_NEGZ);
    vertices[7] = vertex(vec3(-1.0f, -1.0f, -1.0f), RAFGL_GREEN, 1.0f, 0.0f, 1.0f, RAFGL_VEC3_NEGZ);
    vertices[8] = vertex(vec3(1.0f, 1.0f, -1.0f), RAFGL_GREEN, 1.0f, 1.0f, 0.0f, RAFGL_VEC3_NEGZ);

    vertices[9] = vertex(vec3(1.0f, 1.0f, -1.0f), RAFGL_GREEN, 1.0f, 1.0f, 0.0f, RAFGL_VEC3_NEGZ);
    vertices[10] = vertex(vec3(-1.0f, -1.0f, -1.0f), RAFGL_GREEN, 1.0f, 0.0f, 1.0f, RAFGL_VEC3_NEGZ);
    vertices[11] = vertex(vec3(1.0f, -1.0f, -1.0f), RAFGL_BLUE, 1.0f, 1.0f, 1.0f, RAFGL_VEC3_NEGZ);

    //Floor
    vertices[12] = vertex(vec3(-20.0f, -1.0f, -20.0f), RAFGL_GREEN, 1.0f, 0.0f, 0.0f, RAFGL_VEC3_Y);
    vertices[13] = vertex(vec3(-20.0f, -1.0f, 20.0f), RAFGL_GREEN, 1.0f, 0.0f, 10.0f, RAFGL_VEC3_Y);
    vertices[14] = vertex(vec3(20.0f, -1.0f, -20.0f), RAFGL_BLUE, 1.0f, 10.0f, 0.0f, RAFGL_VEC3_Y);

    vertices[15] = vertex(vec3(20.0f, -1.0f, -20.0f), RAFGL_GREEN, 1.0f, 10.0f, 0.0f, RAFGL_VEC3_Y);
    vertices[16] = vertex(vec3(-20.0f, -1.0f, 20.0f), RAFGL_GREEN, 1.0f, 0.0f, 10.0f, RAFGL_VEC3_Y);
    vertices[17] = vertex(vec3(20.0f, -1.0f, 20.0f), RAFGL_BLUE, 1.0f, 10.0f, 10.0f, RAFGL_VEC3_Y);

    //Ceiling
    vertices[18] = vertex(vec3(-20.0f, 2.0f, -20.0f), RAFGL_GREEN, 1.0f, 0.0f, 0.0f, RAFGL_VEC3_NEGY);
    vertices[19] = vertex(vec3(-20.0f, 2.0f, 20.0f), RAFGL_GREEN, 1.0f, 0.0f, 10.0f, RAFGL_VEC3_NEGY);
    vertices[20] = vertex(vec3(20.0f, 2.0f, -20.0f), RAFGL_BLUE, 1.0f, 10.0f, 0.0f, RAFGL_VEC3_NEGY);

    vertices[21] = vertex(vec3(20.0f, 2.0f, -20.0f), RAFGL_GREEN, 1.0f, 10.0f, 0.0f, RAFGL_VEC3_NEGY);
    vertices[22] = vertex(vec3(-20.0f, 2.0f, 20.0f), RAFGL_GREEN, 1.0f, 0.0f, 10.0f, RAFGL_VEC3_NEGY);
    vertices[23] = vertex(vec3(20.0f, 2.0f, 20.0f), RAFGL_BLUE, 1.0f, 10.0f, 10.0f, RAFGL_VEC3_NEGY);

    //Walls
    vertices[24] = vertex(vec3(-20.0f, 2.0f, -20.0f), RAFGL_GREEN, 1.0f, 0.0f, 0.0f, RAFGL_VEC3_X);
    vertices[25] = vertex(vec3(-20.0f, 2.0f, 20.0f), RAFGL_GREEN, 1.0f, 0.0f, 20.0f, RAFGL_VEC3_X);
    vertices[26] = vertex(vec3(-20.0f, -1.0f, -20.0f), RAFGL_BLUE, 1.0f, 2.0f, 0.0f, RAFGL_VEC3_X);

    vertices[27] = vertex(vec3(-20.0f, -1.0f, -20.0f), RAFGL_GREEN, 1.0f, 2.0f, 0.0f, RAFGL_VEC3_X);
    vertices[28] = vertex(vec3(-20.0f, 2.0f, 20.0f), RAFGL_GREEN, 1.0f, 0.0f, 20.0f, RAFGL_VEC3_X);
    vertices[29] = vertex(vec3(-20.0f, -1.0f, 20.0f), RAFGL_BLUE, 1.0f, 2.0f, 20.0f, RAFGL_VEC3_X);

    vertices[30] = vertex(vec3(20.0f, 2.0f, -20.0f), RAFGL_GREEN, 1.0f, 0.0f, 0.0f, RAFGL_VEC3_NEGX);
    vertices[31] = vertex(vec3(20.0f, 2.0f, 20.0f), RAFGL_GREEN, 1.0f, 0.0f, 20.0f, RAFGL_VEC3_NEGX);
    vertices[32] = vertex(vec3(20.0f, -1.0f, -20.0f), RAFGL_BLUE, 1.0f, 2.0f, 0.0f, RAFGL_VEC3_NEGX);

    vertices[33] = vertex(vec3(20.0f, -1.0f, -20.0f), RAFGL_GREEN, 1.0f, 2.0f, 0.0f, RAFGL_VEC3_NEGX);
    vertices[34] = vertex(vec3(20.0f, 2.0f, 20.0f), RAFGL_GREEN, 1.0f, 0.0f, 20.0f, RAFGL_VEC3_NEGX);
    vertices[35] = vertex(vec3(20.0f, -1.0f, 20.0f), RAFGL_BLUE, 1.0f, 2.0f, 20.0f, RAFGL_VEC3_NEGX);

    vertices[36] = vertex(vec3(-20.0f, 2.0f, -20.0f), RAFGL_GREEN, 1.0f, 0.0f, 0.0f, RAFGL_VEC3_Y);
    vertices[37] = vertex(vec3(20.0f, 2.0f, -20.0f), RAFGL_GREEN, 1.0f, 0.0f, 20.0f, RAFGL_VEC3_Y);
    vertices[38] = vertex(vec3(-20.0f, -1.0f, -20.0f), RAFGL_BLUE, 1.0f, 2.0f, 0.0f, RAFGL_VEC3_Y);

    vertices[39] = vertex(vec3(-20.0f, -1.0f, -20.0f), RAFGL_GREEN, 1.0f, 2.0f, 0.0f, RAFGL_VEC3_Y);
    vertices[40] = vertex(vec3(20.0f, 2.0f, -20.0f), RAFGL_GREEN, 1.0f, 0.0f, 20.0f, RAFGL_VEC3_Y);
    vertices[41] = vertex(vec3(20.0f, -1.0f, -20.0f), RAFGL_BLUE, 1.0f, 2.0f, 20.0f, RAFGL_VEC3_Y);

    vertices[42] = vertex(vec3(-20.0f, 2.0f, 20.0f), RAFGL_GREEN, 1.0f, 0.0f, 0.0f, RAFGL_VEC3_NEGY);
    vertices[43] = vertex(vec3(20.0f, 2.0f, 20.0f), RAFGL_GREEN, 1.0f, 0.0f, 20.0f, RAFGL_VEC3_NEGY);
    vertices[44] = vertex(vec3(-20.0f, -1.0f, 20.0f), RAFGL_BLUE, 1.0f, 2.0f, 0.0f, RAFGL_VEC3_NEGY);

    vertices[45] = vertex(vec3(-20.0f, -1.0f, 20.0f), RAFGL_GREEN, 1.0f, 2.0f, 0.0f, RAFGL_VEC3_NEGY);
    vertices[46] = vertex(vec3(20.0f, 2.0f, 20.0f), RAFGL_GREEN, 1.0f, 0.0f, 20.0f, RAFGL_VEC3_NEGY);
    vertices[47] = vertex(vec3(20.0f, -1.0f, 20.0f), RAFGL_BLUE, 1.0f, 2.0f, 20.0f, RAFGL_VEC3_NEGY);

    shader_program_id = rafgl_program_create_from_name("my_shader");
    uni_M = glGetUniformLocation(shader_program_id, "uni_M");
    uni_VP = glGetUniformLocation(shader_program_id, "uni_VP");
    uni_light_color = glGetUniformLocation(shader_program_id, "uni_light_color");
    uni_light_direction = glGetUniformLocation(shader_program_id, "uni_light_direction");
    uni_light_source = glGetUniformLocation(shader_program_id, "uni_light_source");

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vert_count * sizeof(vertex_t), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*) (sizeof(vec3_t)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*) (2 * sizeof(vec3_t)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*) (2 * sizeof(vec3_t) + sizeof(float)));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*) (2 * sizeof(vec3_t) + 3 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}


mat4_t model, view, projection, view_projection;

float fov = 90.0f;

vec3_t camera_position = vec3m(0.0f, 0.0f, 4.0f);
vec3_t camera_target = vec3m(0.0f, 0.0f, 0.0f);
vec3_t camera_up = vec3m(0.0f, 1.0f, 0.0f);
vec3_t aim_dir = vec3m(0.0f, 0.0f, -1.0f);

float camera_angle = -M_PI * 0.5f;
float angle_speed = 0.5f * M_PI, move_speed = 4.0f;

float time = 0;

void main_state_update(GLFWwindow *window, float delta_time, rafgl_game_data_t *game_data, void *args)
{

    time += delta_time;

    if(game_data->keys_down['A']) camera_angle -= angle_speed * delta_time;
    if(game_data->keys_down['D']) camera_angle += angle_speed * delta_time;

    aim_dir = vec3(cosf(camera_angle), 0.0f, sinf(camera_angle));

    if(game_data->keys_down['W']) camera_position = v3_add(camera_position, v3_muls(aim_dir, move_speed * delta_time));
    if(game_data->keys_down['S']) camera_position = v3_add(camera_position, v3_muls(aim_dir, move_speed * delta_time * -1.0f));

    camera_target = v3_add(camera_position, aim_dir);



    float aspect = ((float)(game_data->raster_width)) / game_data->raster_height;
    projection = m4_perspective(fov, aspect, 0.01f, 100.0f);

    view = m4_look_at(camera_position, v3_add(camera_position, aim_dir), camera_up);

    model = m4_identity();

    view_projection = m4_mul(projection, view);
}


void main_state_render(GLFWwindow *window, void *args)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader_program_id);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_doog.tex_id);

    glBindVertexArray(vao);

    glUniformMatrix4fv(uni_M, 1, GL_FALSE, (void*) model.m);
    glUniformMatrix4fv(uni_VP, 1, GL_FALSE, (void*) view_projection.m);

    glUniform3f(uni_light_color, light_color.x, light_color.y, light_color.z);
    glUniform3f(uni_light_direction, light_direction.x, light_direction.y, light_direction.z);
    glUniform3f(uni_light_source, camera_position.x, camera_position.y, camera_position.z);

    glDrawArrays(GL_TRIANGLES, 0, 12);

    glBindTexture(GL_TEXTURE_2D, texture_floor.tex_id);

    glDrawArrays(GL_TRIANGLES, 12, 6);

    glBindTexture(GL_TEXTURE_2D, texture_stone.tex_id);

    glDrawArrays(GL_TRIANGLES, 18, vert_count - 18);

    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}


void main_state_cleanup(GLFWwindow *window, void *args)
{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteShader(shader_program_id);
}
