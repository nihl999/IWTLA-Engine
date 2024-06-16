#pragma once
#include <core/defines.h>
#include <glm/glm.hpp>
// #include <glm/ext.hpp>

namespace World
{
    struct PointLight
    {
        glm::vec3 position;
        glm::vec3 color;
        f32 intensity;
    };

    struct AmbientLight
    {
        glm::vec3 color;
        f32 intensity;
    };

    struct DirectionalLight
    {
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 color;
        f32 intensity;
    };
};