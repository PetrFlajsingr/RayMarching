#version 450

in vec2 texCoord;

layout(binding = 0) uniform sampler2D tex;

layout(location = 0) out vec4 color;

void main()
{
    color = texture(tex, texCoord);
}