#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

out vec2 texCoord;

out vec2 v_rgbNW;
out vec2 v_rgbNE;
out vec2 v_rgbSW;
out vec2 v_rgbSE;
out vec2 v_rgbM;

uniform bool enableFXAA;

// https://github.com/mattdesl/glsl-fxaa
void texcoords(vec2 fragCoord, vec2 resolution,
out vec2 v_rgbNW, out vec2 v_rgbNE,
out vec2 v_rgbSW, out vec2 v_rgbSE,
out vec2 v_rgbM) {
    vec2 inverseVP = 1.0 / resolution.xy;
    v_rgbNW = (fragCoord + vec2(-1.0, -1.0)) * inverseVP;
    v_rgbNE = (fragCoord + vec2(1.0, -1.0)) * inverseVP;
    v_rgbSW = (fragCoord + vec2(-1.0, 1.0)) * inverseVP;
    v_rgbSE = (fragCoord + vec2(1.0, 1.0)) * inverseVP;
    v_rgbM = vec2(fragCoord * inverseVP);
}

void main()
{
    gl_Position = vec4(position, 1.0f);
    if (enableFXAA) {
        texCoord = texcoord * vec2(1288, 691);
        texcoords(texCoord, vec2(1288, 691), v_rgbNW, v_rgbNE, v_rgbSW, v_rgbSE, v_rgbM);
    } else {
        texCoord = texcoord;
    }
}