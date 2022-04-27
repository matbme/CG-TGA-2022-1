#version 450 core
out vec4 color;

in vec2 TexCoord;
flat in int Highlight;

uniform sampler2D boundTexture;

void main()
{
    if (Highlight == 1)
        color = texture(boundTexture, TexCoord) + vec4(0.5, 0.5, 0.5, 0.0);
    else
        color = texture(boundTexture, TexCoord);
}
