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

    // extern const u32 InvalidTextureId;
    // Handle currentHandle;

    // std::unordered_map<std::string, Handle> m_nameHandleMap;
    // // todo custom allocation
    // std::unordered_map<Handle, Resource> m_resources;
    // std::vector<ResourceDescriptor> m_resourceDescriptors;

    Resource *GetResource(std::string rName);
    Resource *GetResource(Handle rId);

    std::string GetResourceFolderPath();
    void LoadResource(Handle h);
    void LoadTextureFromFile(ResourceDescriptor descriptor, Handle h);
    void LoadShadersFromFile(ResourceDescriptor descriptor, Handle h);

    void Init();
    void Cleanup();
};