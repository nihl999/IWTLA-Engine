#include <core/defines.h>
#include <core/resources/resource_system.h>
#include <glm/glm.hpp>
#include <string>

struct Material
{
    std::string shader = "defaults/shaders/unlit";
    glm::vec3 tint;
    ResourceSystem::Handle diffuseMap;
    ResourceSystem::Handle specularMap;
    f32 shininess = 32.0;
};
