#version 330 core

#define NR_POINT_LIGHTS 2 

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float alpha;
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

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
};

out vec4 OutColor;

in vec2 PassTexCoords;
in vec3 PassNormal;
in vec3 PassFragPos;
in vec3 PassCameraPosition;

in vec4 PassLightSpacePosition;

uniform sampler2D Texture0;
uniform sampler2D Texture1; // shadowMap
uniform vec3 lightPosition;
uniform Material material;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLight;
uniform SpotLight spotLight;


vec3 CalculateDirLight(DirLight light, Material material, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalculation(vec4 lightSpacePosition);

void main()
{
    vec3 norm = normalize(PassNormal);
    vec3 viewDir = normalize(PassCameraPosition - PassFragPos);
    vec3 result = CalculateDirLight(dirLight, material, norm, viewDir);
    for(int i = 0; i< NR_POINT_LIGHTS; ++i){
        result += CalculatePointLight(pointLights[i], material, norm, PassFragPos, viewDir);
    }
    result += CalculateSpotLight(spotLight, material, norm, PassFragPos, viewDir);
    OutColor = texture(Texture0, PassTexCoords) * vec4(result,1.0);
}

vec3 CalculateDirLight(DirLight light,Material material, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    //ambient 
    vec3 ambient  = vec3(light.ambient * material.ambient);


    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = vec3(light.diffuse * diff * material.diffuse);


    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(material.specular * spec * light.specular);


    return (ambient + diffuse + specular);
}  

vec3 CalculatePointLight(PointLight light,Material material, vec3 normal, vec3 fragPos, vec3 viewDir){

    vec3 lightDir = normalize(light.position - fragPos);

    //ambient 
    vec3 ambient  = vec3(light.ambient * material.ambient);


    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = vec3(light.diffuse * diff * material.diffuse);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(material.specular * spec * light.specular);


    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));
 
 

    return (ambient + diffuse + specular);
} 

vec3 CalculateSpotLight(SpotLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir){

    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float shadow = ShadowCalculation(PassLightSpacePosition);

    //ambient 
    vec3 ambient  = vec3(light.ambient * material.ambient);


    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = vec3(light.diffuse * diff * material.diffuse);


    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(material.specular * spec * light.specular);


    //soft edges
    float epsilon   = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0) * (1-shadow); 
    diffuse  *= intensity;
    specular *= intensity;

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
            light.quadratic * (distance * distance));    

    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);

}

float ShadowCalculation(vec4 lightSpacePosition)
{
    vec3 projCoords = lightSpacePosition.xyz / lightSpacePosition.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(Texture1, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    vec3 normal = normalize(PassNormal);
    vec3 lightDir = normalize(lightPosition - PassFragPos);
    float bias = max(0.01 * (1.0 - dot(normal, lightDir)), 0.001);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(Texture1, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(Texture1, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}