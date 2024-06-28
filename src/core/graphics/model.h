#pragma once
#include <core/defines.h>
#include <core/resources/resource_system.h>
#include <glm/glm.hpp>
#include <string>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 texCoord;
};

// struct BatchVertex
// {
//     glm::vec3 position;
//     glm::vec3 normal;
//     glm::vec3 texCoord;
//     u32 modelMatrixIndex;
// };

// NOTE: for now, as i haven't planned no proprietary asset filetypes, resource system will only handle Model, so meshes are stored in vector inside model.
// so, multiple model unfortunately can have duplicated meshes
struct Mesh
{
    std::vector<Vertex> vertices;
    ResourceSystem::Handle materialHandle;
    glm::mat4 localTransform = glm::mat4(1);
};

struct Model
{
    std::vector<Mesh> meshes;
    // NOTE: OTHER PROPS
};
