#version 330 core

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

in vec3 FragNormal;
in vec3 FragPos;
in vec3 CameraPos;

uniform Material material;
uniform PointLight pointLights[2];
uniform DirLight dirLight;
uniform SpotLight spotLight;

out vec4 FragColor;

void main(){
    vec3 camera_dir = normalize(CameraPos - FragPos);
    vec3 ref_dir = reflect(-camera_dir, FragNormal);


    // Dir Light

    vec3 dir_ambient = dirLight.ambient * material.ambient;
    vec3 dir_dir = normalize(-dirLight.direction);
    float dir_d_intensity = max(0.0, dot(dir_dir, FragNormal));
    vec3 dir_diffuse = dir_d_intensity * material.diffuse * dirLight.diffuse;
    float dir_s_intensity = pow(min(max(dot(ref_dir, dir_dir), 0.0), 1.0), material.shininess);
    vec3 dir_specular = dir_s_intensity * material.specular * dirLight.specular;


    // Spot Light

    vec3 spot_ambient = spotLight.ambient * material.ambient;
    vec3 spot_dir = normalize(spotLight.position - FragPos);
    float spot_dist = length(spotLight.position - FragPos);
    float spot_d_intensity = dot(spot_dir, FragNormal);
    vec3 spot_diffuse = max(0.0, spot_d_intensity) * material.diffuse * spotLight.diffuse;

    float spot_s_intensity = pow(min(max(dot(spot_dir, ref_dir), 0.0), 1.0), material.shininess);
    vec3 spot_specular = spotLight.specular * material.specular * spot_s_intensity; 
    
    float alignment = dot(-spot_dir, normalize(spotLight.direction));
    float intensity;
    if (alignment > spotLight.cutOff) {
        intensity = 1.0;
    } else if (alignment > spotLight.outerCutOff) {
        intensity = 1.0 - (alignment - spotLight.cutOff) / (spotLight.outerCutOff - spotLight.cutOff);
    } else {
        intensity = 0.0;
    }
    spot_ambient *= intensity;
    spot_diffuse  *= intensity;
    spot_specular *= intensity;

    float spot_attenuation = 1.0 /
        (spotLight.constant + spotLight.linear * spot_dist + spotLight.quadratic * spot_dist * spot_dist);
    spot_ambient *= spot_attenuation;
    spot_diffuse  *= spot_attenuation;
    spot_specular *= spot_attenuation;


    // Point Lights

    vec3 point_ambient = vec3(0, 0, 0);
    vec3 point_diffuse = vec3(0, 0, 0);
    vec3 point_specular = vec3(0, 0, 0);
    for (int i = 0; i < 2; ++i) {
        if (pointLights[i].diffuse == vec3(0, 0, 0)) break;
        vec3 point_dir = normalize(pointLights[i].position - FragPos);
        float point_dist = length(pointLights[i].position - FragPos);
        float point_attenuation = 1.0 /
            (pointLights[i].constant + pointLights[i].linear * point_dist + pointLights[i].quadratic * point_dist * point_dist);
        point_ambient += pointLights[i].ambient * material.ambient * point_attenuation;
        float point_d_intensity = max(0.0, dot(point_dir, FragNormal));
        point_diffuse += point_d_intensity * pointLights[i].diffuse * material.diffuse * point_attenuation;
        float point_s_intensity = pow(min(max(dot(point_dir, ref_dir), 0.0), 1.0), material.shininess);
        point_specular += point_s_intensity * pointLights[i].specular * material.specular * point_attenuation;
    }


    // Join lights

    FragColor = vec4(
        dir_ambient + dir_diffuse + dir_specular +
        spot_ambient + spot_diffuse + spot_specular +
        point_ambient + point_diffuse + point_specular, 1.0
    );
}