#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec2 camera_position;
uniform float camera_rotation;
uniform vec2 camera_scale;

struct Rectangle
{
    vec2 min_corner;
    vec2 size;
    vec3 color;
};

in Rectangle rectangle[];

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
    for (int rect = 0; rect < gl_in.length(); ++rect)
    {
        for (int vert = 0; vert < 4; ++vert)
        {
            Rectangle r = rectangle[rect];
            vec2 position = r.min_corner + vec2(vert < 2, vert % 2) * r.size;
            // 1.0  0.0
            // 1.0  1.0
            // 0.0  0.0
            // 0.0  1.0
            frag_colour = r.color;
            vec2 rotated = (position - camera_position) * rotate(camera_rotation);
            gl_Position = vec4(rotated / camera_scale, 0.0, 1.0);
            EmitVertex();
        }
        EndPrimitive();
    }
}
