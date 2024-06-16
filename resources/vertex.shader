#version 430 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_uv;
layout(location = 0) uniform mat4 vpMatrix;
layout(location = 1) uniform mat4 modelMatrix;
layout(location = 2) uniform float in_ambientLightForce;

out vec3 position;
out vec3 uv;
out float ambientLightForce;

void main() {
    gl_Position = vpMatrix * modelMatrix * vec4(vertexPosition_modelspace, 1.0f);
    position = vertexPosition_modelspace;
    uv = in_uv;
    ambientLightForce = in_ambientLightForce;
}
