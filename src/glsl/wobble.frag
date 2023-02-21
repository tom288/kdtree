#version 330 core

uniform float uv_zoom;
uniform float time;

in vec2 frag_position;
in vec3 frag_colour;

out vec4 colour;

// Adapted https://www.shadertoy.com/view/7sdXzM

#define TAU (6.283185307)

float interp_iter(float t, float a, float b)
{
    float t_inv = 1.0 - t;
    return (
        3.0 * t *     t_inv * t_inv * a +
        3.0 * t * t * t_inv *         b +
              t * t * t
    );
}

float interp(float t, float a, float b)
{
    return interp_iter(
        interp_iter(
            clamp(t, 0.0, 1.0),
            a, b
        ),
        a, b
    );
}

void main()
{
    vec2 uv = frag_position * 0.5 * uv_zoom + 0.5;
    float thres = interp(
        uv.x,
        cos(time * TAU) * 0.5 + 0.5,
        sin(time * TAU) * 0.5 + 0.5
    );
    colour = vec4(mix(
        1.0 - frag_colour,
        frag_colour,
        step(uv.y, thres)
    ), 1.0);
}
