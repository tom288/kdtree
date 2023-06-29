#include "camera.h"

Camera camera_init(vec2 window_size)
{
    return (Camera) {
        .position = { 0.0f, 0.0f },
        .velocity = { 0.0f, 0.0f },
        .base_size = { window_size[0] / window_size[1], 1.0f },
        .rotation = 0.0f,
        .scale_power = 0.0f,
    };
}

void camera_step(
    Camera* camera,
    vec2 movement_input,
    vec2 camera_input,
    float delta_time
) {
    // Scale
    camera->scale_power += camera_input[1] * delta_time;
    const float scale = powf(3.0f, camera->scale_power);
    glm_vec2_scale(camera->base_size, scale, camera->scaled_size);
    camera->speed = glm_vec2_norm(camera->scaled_size) * 0.6f;
    // Rotation
    camera->rotation += camera_input[0] * delta_time * 1.5f;
    // Translation
    vec2 movement_delta;
    glm_vec2_normalize_to(movement_input, movement_delta);
    glm_vec2_rotate(movement_delta, camera->rotation, movement_delta);
    glm_vec2_scale(movement_delta, delta_time * camera->speed, movement_delta);
    glm_vec2_add(movement_delta, camera->position, camera->position);
}

void camera_screen_to_world(Camera camera, vec2 screen_coordinate, vec2 dest)
{
    glm_vec2_mul(screen_coordinate, camera.scaled_size, dest);
    glm_vec2_add(camera.position, dest, dest);
}

void camera_use(Camera camera, Shader shader)
{
    shader_use(shader);
    shader_set_2fv(shader, "camera_position", 1, camera.position);
    shader_set_1f(shader, "camera_rotation", camera.rotation);
    shader_set_2fv(shader, "camera_scale", 1, camera.scaled_size);
}
