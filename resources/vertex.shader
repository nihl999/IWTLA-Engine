#version 430 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_uv;
layout(location = 0) uniform mat4 vpMatrix;
layout(location = 1) uniform mat4 modelMatrix;

out vec3 position;
out vec3 uv;
out vec3 normal;

void main() {
    gl_Position = vpMatrix * modelMatrix * vec4(vertexPosition_modelspace, 1.0);
    position = vec3(modelMatrix * vec4(vertexPosition_modelspace, 1.0));
    uv = in_uv;
    normal = mat3(transpose(inverse(modelMatrix))) * in_normal;
}
