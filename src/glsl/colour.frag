#version 330 core

in vec3 colour_frag;

out vec4 colour;
  
void main()
{
    colour = vec4(colour_frag, 1.0);
}
