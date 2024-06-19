#pragma once
#include <GLAD/glad.h>
#include <string>
#include <core/defines.h>
#include <glm/glm.hpp>

class ShaderProgram
{
public:
    i32 id;

public:
    bool UniformFMat4(std::string variableName, glm::mat4 value);
    bool UniformFVec3(std::string variableName, glm::vec3 value);
    bool UniformF1(std::string variableName, f32 value);
};
