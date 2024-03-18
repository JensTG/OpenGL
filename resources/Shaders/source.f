#version 330 core
#extension GL_ARB_separate_shader_objects : enable

uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(result, 1.0f);
}