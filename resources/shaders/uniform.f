#version 330 core
#extension GL_ARB_separate_shader_objects : enable

out vec4 FragColor;
uniform vec3 uniformColor;

void main()
{
    FragColor = vec4(uniformColor, 1.0f);
}