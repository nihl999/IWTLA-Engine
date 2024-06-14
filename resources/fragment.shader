#version 330 core
out vec4 color;
in vec3 position;
in vec2 uv;
uniform sampler2D ourTexture;
void main() {
    color = texture(ourTexture, uv);
}
