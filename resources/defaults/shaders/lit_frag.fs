#version 430 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
}; 

in vec3 position;
in vec3 uv;
in vec3 normal;

uniform float ambient_light_force;
uniform vec3 point_light_position;
uniform vec3 point_light_color;
uniform vec3 cam_pos;

uniform Material material;

out vec4 color;

void main() {
    vec3 frag_diffuse_tex = vec3(texture(material.diffuse, uv.xy));
    vec3 frag_specular_tex = vec3(texture(material.specular, uv.xy));
    vec3 ambient = 0.1 * frag_diffuse_tex;
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(point_light_position - position);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * ambient_light_force * point_light_color * frag_diffuse_tex;
    vec3 viewDir = normalize(cam_pos - position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float specularStrength = mix(0.0, 0.5, ambient_light_force);
    vec3 specular = specularStrength * spec * point_light_color * frag_specular_tex;   
    color = vec4((ambient + diffuse + specular), 1.0);
}
