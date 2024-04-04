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
uniform DirectLight dLight;
uniform SpotLight sLight;
uniform PointLight pLight;

out vec4 FragColor;

vec3 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    return (ambient + diffuse + specular);
}

void main()
{
    vec3 output = vec3(0.0f);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 norm = normalize(Normal);

    output += calcDirectLight(dLight, norm, viewDir);

    FragColor = vec4(output, 1.0);
}