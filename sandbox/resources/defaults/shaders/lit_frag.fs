#version 430 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
}; 

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float quadratic;
    float linear;

    float intensity;
};

#define MAX_POINT_LIGHTS 10

in vec3 position;
in vec3 uv;
in vec3 normal;

uniform vec3 cam_pos;

uniform Material material;

uniform uint qtd_point_light;
uniform DirectionalLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];


out vec4 color;


vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLights(PointLight light, vec3 normal, vec3 viewDir);

void main() {
    vec3 frag_output = vec3(0.0);
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(cam_pos - position);
    frag_output += CalcDirectionalLight(dirLight, norm, viewDir);
    for(uint i = 0; i<qtd_point_light; i++){
        frag_output += CalcPointLights(pointLights[i], norm, viewDir);
    }
    color = vec4(frag_output, 1.0);
};


vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 frag_diffuse_tex = vec3(texture(material.diffuse, uv.xy));
    vec3 frag_specular_tex = vec3(texture(material.specular, uv.xy));

    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.color  * frag_diffuse_tex * 0.1;
    vec3 diffuse  = light.color  * diff * frag_diffuse_tex;
    vec3 specular = light.color * spec * frag_specular_tex;
    return (ambient + diffuse + specular) * light.intensity;
} 

vec3 CalcPointLights(PointLight light, vec3 normal, vec3 viewDir)
{
    vec3 frag_diffuse_tex = vec3(texture(material.diffuse, uv.xy));
    vec3 frag_specular_tex = vec3(texture(material.specular, uv.xy));

    vec3 lightDir = normalize(light.position - position);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.color  * frag_diffuse_tex * 0.1;
    vec3 diffuse  = light.color  * diff * frag_diffuse_tex;
    vec3 specular = light.color * spec * frag_specular_tex;
    float lightDistance = length(light.position - position);
    float attenuation = 1.0 / (1 + light.linear * lightDistance + 
    		    light.quadratic * (lightDistance * lightDistance));   
    return (ambient + diffuse + specular) * light.intensity * attenuation;
} 