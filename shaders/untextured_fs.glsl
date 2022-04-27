#version 450 core
out vec4 color;

in vec4 finalColor;
flat in int Highlight;

void main()
{
    if (Highlight == 1)
        color = finalColor + vec4(0.5, 0.5, 0.5, 0.0);
    else
        color = finalColor;
}
