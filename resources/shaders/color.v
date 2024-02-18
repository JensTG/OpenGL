#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 aPos;
out vec4 ourColor;

void main() 
{
    gl_Position = vec4(aPos, 1.0f);
    ourColor = vec4((aPos * 0.5f) + 0.5f, 1.0f);
}