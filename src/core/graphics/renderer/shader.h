#pragma once
#include <GLAD/glad.h>
#include <string>
#include <core/defines.h>
#include <glm/glm.hpp>

const i32 InvalidShaderId = -999;

struct ShaderProgram
{
    i32 id = InvalidShaderId;
};
