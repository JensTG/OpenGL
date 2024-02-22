#version 330 core
#extension GL_ARB_separate_shader_objects : enable

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform vec3 uniformColor;

void main()
{
    FragColor = texture(texture1, TexCoord) * vec4(uniformColor, 1.0f);
}