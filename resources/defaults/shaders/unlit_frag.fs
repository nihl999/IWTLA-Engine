#version 430 core

in vec3 uv;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
}; 

uniform vec3 point_light_color;
uniform Material material;

out vec4 color;

void main() {
    vec3 frag_diffuse_tex = vec3(texture(material.diffuse, uv.xy));
    color = vec4(frag_diffuse_tex, 1);
}
