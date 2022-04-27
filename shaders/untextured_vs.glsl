#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec4 finalColor;
out int Highlight;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int texHighlight;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    finalColor = vec4(color,1.0);
    Highlight = texHighlight;
}
