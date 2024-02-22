#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTex;

uniform mat4 proj;
uniform mat4 model;
uniform mat4 view;

out vec4 ourColor;
out vec2 TexCoord;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0f);
    ourColor = vec4(aPos * 0.5 + 0.5, 1.0f);
    TexCoord = aTex;
} 