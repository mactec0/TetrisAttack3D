#version 330 core

struct Material {
    float shininess;
};

struct DirLight {
    int state;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    int state;
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


#define POINT_LIGHT_MAX 16
#define DIR_LIGHT_MAX 4

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;


uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform vec3 colorModulation;


uniform Material material;
uniform vec3 viewPos;
uniform PointLight pointLights[POINT_LIGHT_MAX];
uniform DirLight dirLights[DIR_LIGHT_MAX];

out vec4 frag_color;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main() {
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result= vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < DIR_LIGHT_MAX; i++){
        if(dirLights[i].state==1){
            result +=CalcDirLight(dirLights[i], normal, viewDir);
        }
    }

    for(int i = 0; i < POINT_LIGHT_MAX; i++){
        if(pointLights[i].state==1){
            result += CalcPointLight(pointLights[i], normal, FragPos, viewDir);
        }
    }

    frag_color = vec4(result*colorModulation, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(diffuseMap, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuseMap, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(specularMap, TexCoord));
    return (ambient + diffuse + specular);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * vec3(texture(diffuseMap, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuseMap, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(specularMap, TexCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
