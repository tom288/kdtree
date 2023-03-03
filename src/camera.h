#pragma once

#include "shader.h"
#include <cglm/cglm.h>

/// @brief Orthographic camera
typedef struct Camera {
    // Kinematics
    vec2 position;
    vec2 velocity;
    // Display
    vec2 size;
    float rotation;
} Camera;

/// @brief Create a camera for a given window size
/// @param window_size Size of window
/// @return Camera created
Camera camera_init(vec2 window_size);

/// @brief Move a camera given input and the time between the last 2 frames
/// @param camera Camera to move
/// @param movement_input Movement input from window
/// @param delta_time Time between the last 2 frames
void camera_step(Camera* camera, vec2 movement_input, float delta_time);

/// @brief Convert a camera screen coordinate to a world coordinate
/// @param camera Camera to use
/// @param screen_coordinate Screen coordinate
/// @param dest Destination for calculation result
void camera_screen_to_world(Camera camera, vec2 screen_coordinate, vec2 dest);

/// @brief Use a particular camera for a particular shader
/// @param camera Camera to use
/// @param shader Shader to use camera for
void camera_use(Camera camera, Shader shader);
