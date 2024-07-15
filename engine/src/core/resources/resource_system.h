#pragma once
#include <core/defines.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace ResourceSystem
{
    enum ResourceType
    {
        RESOURCE_TEXTURE,
        RESOURCE_MATERIAL,
        RESOURCE_MESH,
        RESOURCE_MODEL,
        RESOURCE_SHADER,
        RESOURCE_TYPE_COUNT
    };
    struct ResourceDescriptor
    {
        std::string path;
        std::string name;
        ResourceType type;
    };

    struct Resource
    {
        void *data;
    };

    typedef u16 Handle;

    Resource *GetResource(std::string rName);
    Resource *GetResource(Handle rId);

    Handle PrepareResource(const ResourceDescriptor resourceDescriptor);

    // Implementation and Internals
    // ------------
    //  std::string GetResourceFolderPath();
    //  void LoadResource(Handle h);
    //  void LoadTextureFromFile(ResourceDescriptor descriptor, Handle h);
    //  void LoadShadersFromFile(ResourceDescriptor descriptor, Handle h);
    //  void LoadModelFromFile(ResourceDescriptor descriptor, Handle h);
    //  void ProcessMesh(aiMesh *mesh, glm::mat4 meshTransformation);
    //  void TraverseNodes(aiNode *node, glm::mat4 transform);
    //  void GetTotalMeshes(aiNode *node);
    //--------------

    void Init();
    void Cleanup();
};