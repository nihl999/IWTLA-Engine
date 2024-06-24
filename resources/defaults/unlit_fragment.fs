#version 430 core

in vec3 uv;

uniform sampler2D ourTexture;

uniform vec3 point_light_color;

out vec4 color;

void main() {
    color = texture(ourTexture, uv.xy) * vec4(point_light_color, 1);
}
