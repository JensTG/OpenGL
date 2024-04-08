#version 330 core
#define MAX_LIGHTS_SPOT 8

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
};

struct SpotLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
};

in vec2 TexCoords;
in vec3 FragPos;  
in vec3 Normal;  
  
uniform vec3 viewPos;
uniform Material material;
uniform SpotLight light;

out vec4 FragColor;

void main()
{
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(light.position - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * intensity * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * intensity * spec * vec3(texture(material.specular, TexCoords)); 

    vec3 output = ambient + diffuse + specular;
    FragColor = vec4(output, 1.0);
}