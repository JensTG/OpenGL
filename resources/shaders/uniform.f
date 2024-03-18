#version 330 core
#extension GL_ARB_separate_shader_objects : enable

in vec4 ourColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = lightColor * ambientStrength;

    vec3 result = ambient * objectColor;
    FragColor = vec4(result, 1.0f);
}