#version 420 core

layout(location = 0) in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D ourTex;

void main()
{
    fragColor = texture(ourTex, vec2(texCoord.x, texCoord.y));
}