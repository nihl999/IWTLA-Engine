#version 430 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 in_uv;
layout(location = 2) uniform mat4 vpMatrix;
layout(location = 3) uniform mat4 modelMatrix;
out vec3 position;
out vec2 uv;
void main() {
    gl_Position = vpMatrix * modelMatrix * vec4(vertexPosition_modelspace, 1.0f);
    position = vertexPosition_modelspace;
    uv = in_uv;
}
