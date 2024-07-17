#pragma once
#include <core/defines.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

const i32 InvalidShaderId = -999;

struct OUROAPI ShaderProgram {
  i32 id = InvalidShaderId;
};
