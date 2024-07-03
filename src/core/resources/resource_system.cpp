#include <core/resources/resource_system.h>
#include <common/stb_image.h>
#include <common/File/File.h>
#include <core/graphics/renderer/renderer.h>
#include <core/graphics/model.h>
#include <core/graphics/renderer/shader.h>
#include <core/graphics/renderer/renderer.h>
#include <core/assert.h>

namespace ResourceSystem
{
    // todo make base resource folder changeable
    std::string GetResourceFolderPath()
    {
        return "../resources/";
    }

    Handle currentHandle = 0;

    // NOTE: based on https://github.com/ekardnam/Newtonic/
    std::unordered_map<std::string, Handle> m_nameHandleMap = {};
    std::unordered_map<Handle, Resource> m_resources = {};
    std::vector<ResourceDescriptor> m_resourceDescriptors = {};

    Handle PrepareResource(const ResourceDescriptor resourceDescriptor)
    {
        Handle h = currentHandle;
        // NOTE: Assumes that names are unique...
        if (m_nameHandleMap.find(resourceDescriptor.name) != m_nameHandleMap.end())
        {
            return m_nameHandleMap[resourceDescriptor.name];
        }
        m_resourceDescriptors.push_back(resourceDescriptor);
        m_nameHandleMap[resourceDescriptor.name] = h;
        currentHandle++;
        return h;
    }

    // todo reference counting?
    Resource *GetResource(Handle h)
    {
        // ASSERT(h < currentHandle);
        if (m_resources.find(h) == m_resources.end())
            LoadResource(h);
        return &m_resources[h];
    }
    Resource *GetResource(std::string rName)
    {
        // ASSERT(m_nameHandleMap.find(rName) != m_nameHandleMap.end())
        return GetResource(m_nameHandleMap[rName]);
    }

    // NOTE: basically a factory?
    void LoadResource(Handle handle)
    {
        ResourceDescriptor descriptor = m_resourceDescriptors[handle];
        switch (descriptor.type)
        {
        case RESOURCE_MATERIAL:
            return;
        case RESOURCE_MESH:
            return;
        case RESOURCE_MODEL:
            return;
        case RESOURCE_SHADER:
            LoadShadersFromFile(descriptor, handle);
            return;
        case RESOURCE_TEXTURE:
            LoadTextureFromFile(descriptor, handle);
            return;

        default:
            break;
        }
    }

    void LoadTextureFromFile(ResourceDescriptor descriptor, Handle h)
    {

        Texture *texture = new Texture;
        std::string filepath = GetResourceFolderPath().append(descriptor.path);

        byte *data = stbi_load(filepath.c_str(), &texture->width, &texture->height, (i32 *)&texture->nrChannels, STBI_rgb_alpha);
        if (!data)
        {
            const char *failureReason = stbi_failure_reason();
            printf("error loading texture: %s\n", failureReason);
            exit(1);
        }

        texture->data = data;

        Renderer::GetInstance().CreateGPUTexture(*texture);

        printf("texture: %s loaded successfully, id: %d\n", descriptor.path.c_str(), texture->id);

        m_resources[h] = {
            .data = texture};
    };

    void LoadShadersFromFile(ResourceDescriptor descriptor, Handle h)
    {
        ShaderProgram *shaderProgram = new ShaderProgram();
        std::string vertexShaderFile = descriptor.path;
        vertexShaderFile.append("_vert.vs");
        std::string fragmentShaderFile = descriptor.path;
        fragmentShaderFile.append("_frag.fs");

        // todo Make good resource system file reading
        char *vertexShader = FileUtils::readFile(vertexShaderFile.c_str());
        char *fragmentShader = FileUtils::readFile(fragmentShaderFile.c_str());

        Renderer::GetInstance().CreateShaderProgram(vertexShader, fragmentShader, *shaderProgram);
        free(vertexShader);
        free(fragmentShader);
        m_resources[h] = {
            .data = shaderProgram};
    }

    void LoadHardcodedCubeMeshTextured(Handle h);
    void LoadHardcodedCubeMesh(Handle h);

    void Init()
    {
        stbi_set_flip_vertically_on_load(1);
        Handle defaultWhite = PrepareResource({.path = "defaults/textures/white_1x1.jpg", .name = "defaults/textures/white", .type = RESOURCE_TEXTURE});
        Handle defaultLit = PrepareResource({.path = "defaults/shaders/lit", .name = "defaults/shaders/lit", .type = RESOURCE_SHADER});
        Handle defaultUnlit = PrepareResource({.path = "defaults/shaders/unlit", .name = "defaults/shaders/unlit", .type = RESOURCE_SHADER});
        LoadResource(defaultWhite);
        LoadResource(defaultLit);
        LoadResource(defaultUnlit);

        // Hardcoded loading
        Handle defaultCube = PrepareResource({.path = "hardcoded/cube", .name = "defaults/model/cube_tex", .type = RESOURCE_MODEL});
        Handle defaultUntextured = PrepareResource({.path = "hardcoded/cube", .name = "defaults/model/cube", .type = RESOURCE_MODEL});
        LoadHardcodedCubeMeshTextured(defaultCube);
        LoadHardcodedCubeMesh(defaultUntextured);
    };
    // todo Cleanup function

    void LoadHardcodedCubeMeshTextured(Handle h)
    {
        const float cube_vert_pos[108] = {
            -0.5f, -0.5f, -0.5f, //
            0.5f, -0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, -0.5f};
        const float cube_vert_uvs[72] = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f};
        const float cube_vert_normals[108] = {
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,

            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,

            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,

            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,

            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,

            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f};

        Graphics::Mesh cubeMesh;
        Handle diffuse = PrepareResource({.path = "container.png", .name = "container_diffuse", .type = RESOURCE_TEXTURE});
        Handle specular = PrepareResource({.path = "container_specular.png", .name = "container_specular", .type = RESOURCE_TEXTURE});

        std::vector<Graphics::Vertex> vertices = std::vector<Graphics::Vertex>();
        vertices.reserve(36);
        for (u32 i = 0; i < 36; i++)
        {
            Graphics::Vertex vert;
            vert.position = glm::vec3(cube_vert_pos[i * 3], cube_vert_pos[i * 3 + 1], cube_vert_pos[i * 3 + 2]);
            vert.normal = glm::vec3(cube_vert_normals[i * 3], cube_vert_normals[i * 3 + 1], cube_vert_normals[i * 3 + 2]);
            vert.texCoord = glm::vec3(cube_vert_uvs[i * 2], cube_vert_uvs[i * 2 + 1], 1);
            vertices.push_back(vert);
        }

        cubeMesh.vertices = vertices;
        Handle mat = PrepareResource({.path = "hardcoded/textured_cube_mat", .name = "hardcoded/textured_cube_mat", .type = RESOURCE_MATERIAL});
        Material *material = new Material{.shader = "defaults/shaders/lit",
                                          .tint = glm::vec3(1),
                                          .diffuseMap = diffuse,
                                          .specularMap = specular,
                                          .shininess = 64};
        m_resources[mat] = {.data = material};
        cubeMesh.material = mat;

        // Handle defaultCubeMesh = PrepareResource({.path = "hardcoded/cube", .name = "defaults/mesh/cube_tex", .type = RESOURCE_MESH});
        std::vector<Graphics::Mesh> meshes = std::vector<Graphics::Mesh>();
        meshes.push_back(cubeMesh);
        Graphics::Model *model = new Graphics::Model{
            .meshes = meshes};
        // m_resources[defaultCubeMesh] = {.data = cubeMesh};
        m_resources[h] = {
            .data = model};
    }
    void LoadHardcodedCubeMesh(Handle h)
    {
        const float cube_vert_pos[108] = {
            -0.5f, -0.5f, -0.5f, //
            0.5f, -0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, -0.5f};
        const float cube_vert_uvs[72] = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f};
        const float cube_vert_normals[108] = {
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,

            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,

            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,

            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,

            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,

            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f};

        Graphics::Mesh cubeMesh;
        Handle diffuse = m_nameHandleMap["defaults/textures/white"];
        Handle specular = m_nameHandleMap["defaults/textures/white"];

        std::vector<Graphics::Vertex> vertices = std::vector<Graphics::Vertex>();
        vertices.reserve(36);

        for (u32 i = 0; i < 36; i++)
        {
            Graphics::Vertex vert;
            vert.position = glm::vec3(cube_vert_pos[i * 3], cube_vert_pos[i * 3 + 1], cube_vert_pos[i * 3 + 2]);
            vert.normal = glm::vec3(cube_vert_normals[i * 3], cube_vert_normals[i * 3 + 1], cube_vert_normals[i * 3 + 2]);
            vert.texCoord = glm::vec3(cube_vert_uvs[i * 2], cube_vert_uvs[i * 2 + 1], 1);
            vertices.push_back(vert);
        }

        cubeMesh.vertices = vertices;
        Handle mat = PrepareResource({.path = "hardcoded/untextured_cube_mat", .name = "hardcoded/untextured_cube_mat", .type = RESOURCE_MATERIAL});
        Material *material = new Material{.tint = glm::vec3(1),
                                          .diffuseMap = diffuse,
                                          .specularMap = specular};
        m_resources[mat] = {.data = material};
        cubeMesh.material = mat;

        std::vector<Graphics::Mesh> meshes = std::vector<Graphics::Mesh>();
        meshes.push_back(cubeMesh);
        Graphics::Model *model = new Graphics::Model{
            .meshes = meshes};
        // m_resources[defaultCubeMesh] = {.data = cubeMesh};
        m_resources[h] = {
            .data = model};

        //     m_resources[h] = {
        //         .data = cubeMesh};
    }
}
