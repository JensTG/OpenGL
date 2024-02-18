#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 transform;
out vec3 ourColor;

void main()
{
    vec4 res = transform * vec4(aPos, 1.0f);
    gl_Position = transform * vec4(aPos, 1.0f);
    ourColor = aPos * 0.5 + 0.5;
} 