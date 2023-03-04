#include "camera.h"

Camera camera_init(vec2 window_size)
{
    Camera camera = {
        .position = { 0.0f, 0.0f },
        .velocity = { 0.0f, 0.0f },
        .size = { 1.0f, 1.0f },
        .rotation = 0.0f,
    };
    camera.size[0] *= window_size[0] / window_size[1];
    return camera;
}

void camera_step(Camera* camera, vec2 movement_input, float delta_time)
{
    vec2 movement_delta;
    glm_vec2_normalize_to(movement_input, movement_delta);
    const float velocity = delta_time * glm_vec2_norm(camera->size);
    glm_vec2_scale(movement_delta, velocity, movement_delta);
    glm_vec2_add(movement_delta, camera->position, camera->position);
}

void camera_screen_to_world(Camera camera, vec2 screen_coordinate, vec2 dest)
{
    glm_vec2_mul(screen_coordinate, camera.size, dest);
    glm_vec2_add(camera.position, dest, dest);
}

void camera_use(Camera camera, Shader shader)
{
    shader_set_2fv(shader, "camera_position", 1, camera.position);
    shader_set_2fv(shader, "camera_scale", 1, camera.size);
}
