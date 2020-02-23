#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

out vec2 v2fTexcoords;

void main()
{
    gl_Position = vec4(position, 1.0f);
    v2fTexcoords = texcoord;
}