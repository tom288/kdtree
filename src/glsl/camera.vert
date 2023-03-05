#version 330 core

uniform vec2 camera_position;
uniform float camera_rotation;
uniform vec2 camera_scale;

in vec2 position;
in vec3 colour;

out vec3 frag_colour;

// Obtain matrix for clockwise rotation by angle radians
mat2 rotate(float angle)
{
    return mat2(
        cos(angle), sin(angle),
        -sin(angle), cos(angle)
    );
}

void main()
{
    gl_Position = vec4((position - camera_position) / camera_scale, 0.0, 1.0);
    gl_Position.xy *= rotate(camera_rotation);
    frag_colour = colour;
}
