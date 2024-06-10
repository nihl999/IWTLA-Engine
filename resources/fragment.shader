#version 330 core
out vec3 color;
in vec3 position;
void main() {
    color = vec3(position.x, position.y, position.z);
}
