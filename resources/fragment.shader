#version 430 core

in vec3 position;
in vec3 uv;
in float ambientLightForce;
uniform sampler2D ourTexture;

out vec4 color;

void main() {
    color = texture(ourTexture, uv.xy);
    color = color * ambientLightForce;
}
