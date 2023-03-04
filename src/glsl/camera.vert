#version 330 core

uniform vec2 camera_position;
uniform vec2 camera_scale;

in vec2 position;
in vec3 colour;

out vec3 frag_colour;

void main()
{
    gl_Position = vec4((position - camera_position) / camera_scale, 0.0, 1.0);
    frag_colour = colour;
}
