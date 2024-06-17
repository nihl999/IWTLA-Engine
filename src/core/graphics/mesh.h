#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <core/defines.h>

namespace Graphics
{
    const i32 invalidTexId = -999;
    struct MeshVertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 texCoord;
    };

    struct Texture
    {
        u32 id = invalidTexId;
        i32 width;
        i32 height;
        i8 nrChannels;
        std::shared_ptr<byte *> data;
    };

    struct Mesh
    {
    public:
        // todo im dumb
        MeshVertex *vertices;
        u32 verticeCount = 0;
        // todo implement texture and material resources ---
        std::string textureName;
    };
}