#version 330 core

in vec3 frag_colour;

out vec4 colour;

void main()
{
    colour = vec4(frag_colour, 1.0);
}
