#version 330 core

in vec2 position;
in vec3 colour;

out vec3 frag_colour;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    frag_colour = colour;
}
