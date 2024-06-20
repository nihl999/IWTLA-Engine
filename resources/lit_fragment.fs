#version 430 core

in vec3 position;
in vec3 uv;
in vec3 normal;
 uniform float ambient_light_force;
 uniform vec3 point_light_position;
uniform vec3 point_light_color;
uniform vec3 cam_pos;


uniform sampler2D ourTexture;

out vec4 color;

void main() {
    vec3 ambient = ambient_light_force * point_light_color;
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(point_light_position - position);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * point_light_color;
    vec3 viewDir = normalize(cam_pos - position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    float specularStrength = 0.5;
    vec3 specular = specularStrength * spec * point_light_color;   
    color = texture(ourTexture, uv.xy);
    color = vec4(color.xyz * (ambient + diffuse + specular), 1.0);
}
