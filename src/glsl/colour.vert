#version 330 core

in vec2 position;
in vec3 colour;

out vec3 colour_frag;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    colour_frag = colour;
}
