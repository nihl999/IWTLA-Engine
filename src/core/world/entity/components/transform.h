#pragma once
#include <core/defines.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Components
{
    struct Transform
    {
        glm::vec3 position = glm::vec3(0);
        glm::vec3 scale = glm::vec3(1);
        glm::vec3 rotation = glm::vec3(0);
        glm::mat4 modelMatrix = glm::mat4(1);
    };
};
