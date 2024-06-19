#version 430 core

in vec3 position;
in vec3 uv;
in vec3 normal;
layout(location = 2) uniform float ambientLightForce;
layout(location = 3) uniform vec3 lightPos;
layout(location = 4) uniform vec3 lightColor;
layout(location = 5) uniform vec3 camPos;


uniform sampler2D ourTexture;

out vec4 color;

void main() {
    vec3 ambient = ambientLightForce * lightColor;
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - position);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 viewDir = normalize(camPos - position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    float specularStrength = 0.5;
    vec3 specular = specularStrength * spec * lightColor;   
    color = texture(ourTexture, uv.xy);
    color = vec4(color.xyz * (ambient + diffuse + specular), 1.0);
}
