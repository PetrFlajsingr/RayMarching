#version 450

in vec2 texCoord;

layout(binding = 0) uniform sampler2D tex;

layout(location = 0) out vec4 color;

in vec2 v_rgbNW;
in vec2 v_rgbNE;
in vec2 v_rgbSW;
in vec2 v_rgbSE;
in vec2 v_rgbM;

uniform bool enableFXAA;
uniform bool asHeatMap;

// https://github.com/mattdesl/glsl-fxaa
#ifndef FXAA_REDUCE_MIN
#define FXAA_REDUCE_MIN   (1.0/ 128.0)
#endif
#ifndef FXAA_REDUCE_MUL
#define FXAA_REDUCE_MUL   (1.0 / 8.0)
#endif
#ifndef FXAA_SPAN_MAX
#define FXAA_SPAN_MAX     8.0
#endif
vec4 fxaa(sampler2D tex, vec2 fragCoord, vec2 resolution,
vec2 v_rgbNW, vec2 v_rgbNE,
vec2 v_rgbSW, vec2 v_rgbSE,
vec2 v_rgbM) {
    vec4 color;
    mediump vec2 inverseVP = vec2(1.0 / resolution.x, 1.0 / resolution.y);
    vec3 rgbNW = texture2D(tex, v_rgbNW).xyz;
    vec3 rgbNE = texture2D(tex, v_rgbNE).xyz;
    vec3 rgbSW = texture2D(tex, v_rgbSW).xyz;
    vec3 rgbSE = texture2D(tex, v_rgbSE).xyz;
    vec4 texColor = texture2D(tex, v_rgbM);
    vec3 rgbM  = texColor.xyz;
    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbM, luma);
    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    mediump vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) *
    (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);

    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX),
    max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
    dir * rcpDirMin)) * inverseVP;

    vec3 rgbA = 0.5 * (
    texture2D(tex, fragCoord * inverseVP + dir * (1.0 / 3.0 - 0.5)).xyz +
    texture2D(tex, fragCoord * inverseVP + dir * (2.0 / 3.0 - 0.5)).xyz);
    vec3 rgbB = rgbA * 0.5 + 0.25 * (
    texture2D(tex, fragCoord * inverseVP + dir * -0.5).xyz +
    texture2D(tex, fragCoord * inverseVP + dir * 0.5).xyz);

    float lumaB = dot(rgbB, luma);
    if ((lumaB < lumaMin) || (lumaB > lumaMax))
    color = vec4(rgbA, texColor.a);
    else
    color = vec4(rgbB, texColor.a);
    return color;
}

vec3 intensityToHeatMap1(float intensity) {
    float aR = 0;   float aG = 0; float aB=1;// RGB for our 1st color (blue in this case).
    float bR = 1; float bG = 0; float bB=0;// RGB for our 2nd color (red in this case).
    const float r   = float((bR - aR) * intensity + aR);// Evaluated as -255*value + 255.
    const float g = float((bG - aG) * intensity + aG);// Evaluates as 0.
    const float b  = float((bB - aB) * intensity + aB);// Evaluates as 255*value + 0.
    return vec3(r, g, b);
}
vec3 intensityToHeatMap2(float intensity) {
    const int NUM_COLORS = 4;
    const vec3 color[NUM_COLORS] = vec3[NUM_COLORS](vec3(0, 0, 1), vec3(0, 1, 0), vec3(1, 1, 0), vec3(1, 0, 0));

    int idx1;
    int idx2;
    float fractBetween = 0;

    if (intensity <= 0)      { idx1 = idx2 = 0; }// accounts for an input <=0
    else if (intensity >= 1)  { idx1 = idx2 = NUM_COLORS-1; }// accounts for an input >=0
    else
    {
        intensity = intensity * (NUM_COLORS-1);// Will multiply intensity by 3.
        idx1  = int(floor(intensity));
        idx2  = idx1+1;// ... and before this index (inclusive).
        fractBetween = intensity - float(idx1);// Distance between the two indexes (0-1).
    }

    const float r = (color[idx2][0] - color[idx1][0])*fractBetween + color[idx1][0];
    const float g = (color[idx2][1] - color[idx1][1])*fractBetween + color[idx1][1];
    const float b = (color[idx2][2] - color[idx1][2])*fractBetween + color[idx1][2];
    return vec3(r, g, b);
}



void main()
{
    if (enableFXAA) {
        color = fxaa(tex, texCoord, vec2(1288, 691), v_rgbNW, v_rgbNE, v_rgbSW, v_rgbSE, v_rgbM);
    } else {
        if (asHeatMap) {
            color = vec4(intensityToHeatMap2(texture(tex, texCoord).r), 1);
        } else {
            color = texture(tex, texCoord);
        }
    }
}