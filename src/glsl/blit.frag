#version 330 core

in vec2 frag_uv;

out vec4 colour;

uniform sampler2D screen_texture;

void main()
{
    colour = texture(screen_texture, frag_uv);
}
