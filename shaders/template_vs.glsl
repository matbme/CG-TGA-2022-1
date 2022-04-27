#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
flat out int Highlight;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int texHighlight;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    TexCoord = aTexCoord;
    Highlight = texHighlight;
}
