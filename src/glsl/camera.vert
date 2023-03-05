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
    vec2 rotated = (position - camera_position) * rotate(camera_rotation);
    gl_Position = vec4(rotated / camera_scale, 0.0, 1.0);
    frag_colour = colour;
}
