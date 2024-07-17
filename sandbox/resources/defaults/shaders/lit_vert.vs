#version 430 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_uv;
uniform mat4 view_projection_matrix;
uniform mat4 model_matrix;

out vec3 position;
out vec3 uv;
out vec3 normal;

void main() {
    gl_Position = view_projection_matrix * model_matrix * vec4(vertexPosition_modelspace, 1.0);
    position = vec3(model_matrix * vec4(vertexPosition_modelspace, 1.0));
    uv = in_uv;
    normal = mat3(transpose(inverse(model_matrix))) * in_normal;
}
