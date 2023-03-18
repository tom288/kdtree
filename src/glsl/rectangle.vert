#version 330 core

in vec2 min_corner;
in vec2 size;
in vec3 colour;

struct Rectangle
{
    vec2 min_corner;
    vec2 size;
    vec3 color;
};

out Rectangle rectangle;

void main()
{
    rectangle.min_corner = min_corner;
    rectangle.size = size;
    rectangle.color = colour;
}
