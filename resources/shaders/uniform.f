#version 330 core
#extension GL_ARB_separate_shader_objects : enable

in vec4 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform vec3 uniformColor;

void main()
{
    FragColor = ourColor;
}