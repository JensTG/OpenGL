#version 330 core
#define MAX_LIGHTS_SPOT 8
#define MAX_LIGHTS_DIRECT 8
#define MAX_LIGHTS_POINT 32

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec2 TexCoords;
in vec3 FragPos;  
in vec3 Normal;  
  
uniform vec3 viewPos;
uniform Material material;
uniform PointLight pLight;

out vec4 FragColor;

vec3 calcPointLight(PointLight light, vec3 norm, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    vec3 ambient = light.ambient * attenuation * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * attenuation * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * attenuation * spec * vec3(texture(material.specular, TexCoords)); 
    
    return (ambient + diffuse + specular);
}

void main()
{
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 norm = normalize(Normal);

    FragColor = vec4(calcPointLight(pLight, norm, viewDir), 1.0f);
}