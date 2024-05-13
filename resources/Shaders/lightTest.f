#version 330 core
#define MAX_LIGHTS_SPOT 8
#define MAX_LIGHTS_DIRECT 8
#define MAX_LIGHTS_POINT 32

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
};

struct DirectLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float strength;
};
struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float strength;
};
struct SpotLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float strength;
};

in vec2 TexCoords;
in vec3 FragPos;  
in vec3 Normal;  
  
uniform vec3 viewPos;
uniform Material material;
uniform DirectLight dLights[MAX_LIGHTS_DIRECT];
uniform SpotLight sLights[MAX_LIGHTS_SPOT];
uniform PointLight pLights[MAX_LIGHTS_POINT];

out vec4 FragColor;

vec3 calcDirLight(DirectLight light, vec3 norm, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
  	
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    return (light.strength * (ambient + diffuse + specular));
}

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
    
    return max((light.strength * (ambient + diffuse + specular)), 0.0);
}

vec3 calcSpotLight(SpotLight light, vec3 norm, vec3 viewDir) {
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

    return (light.strength * (ambient + diffuse + specular));
}

void main()
{
    vec3 output = vec3(0.0f);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 norm = normalize(Normal);

    for(int i = 0; i < MAX_LIGHTS_DIRECT; i++)
        output += calcDirLight(dLights[i], norm, viewDir);

    for(int i = 0; i < MAX_LIGHTS_POINT; i++)
        output += calcPointLight(pLights[i], norm, viewDir);

    for(int i = 0; i < MAX_LIGHTS_SPOT; i++)
        output += calcSpotLight(sLights[i], norm, viewDir);

    FragColor = vec4(output, 1.0);
}