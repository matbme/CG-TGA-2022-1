#version 450 core
out vec4 color;

in vec2 TexCoord;

uniform sampler2D boundTexture;

void main()
{
    color = texture(boundTexture, TexCoord);
}
