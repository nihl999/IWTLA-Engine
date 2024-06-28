#include <core/defines.h>
#include <core/graphics/mesh.h>
#include <core/resources/resource_manager.h>

#include <string>
#include <vector>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

// todo this is bad
// and it only handles simple model with no transform...
// really, i have to study the assimp node structure
// probably, with resource manager, load all scene materials and textures, then meshes, and models only point point points
Model *LoadModel(std::string fileName)
{
    ResourceManager &resourceManager = ResourceManager::GetInstance();
    std::string newPath = resourceManager.GetResourceFolderPath().append(fileName);
    const aiScene *scene = aiImportFile(newPath.c_str(), aiProcessPreset_TargetRealtime_Fast);

    if (!scene)
    {
        const char *error = aiGetErrorString();
        printf("error loading model kaboom %d\n", error);
        exit(1);
    }

    std::vector<Mesh> meshes;
    meshes.reserve(scene->mNumMeshes);

    for (u32 meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
    {
        aiMesh *mesh = scene->mMeshes[meshIndex];

        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        f32 shine;

        aiString name;
        material->Get(AI_MATKEY_NAME, name);
        aiReturn texFound = material->GetTexture(aiTextureType_DIFFUSE, mesh->mMaterialIndex, &name);

        // todo someday will break - is broken already ....
        ResourceId diff = 1;
        ResourceId spec = 1;
        // todo if dont have, start with 0 - default white 1x1 texture
        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString path;
            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == aiReturn_SUCCESS)
            {
                std::string fullPath = path.data;
                printf("diffuse path: %s\n", fullPath.c_str());
                diff = resourceManager.LoadTextureFromFile(fullPath);
            }
        }
        if (material->GetTextureCount(aiTextureType_SPECULAR) > 0)
        {
            aiString path;
            if (material->GetTexture(aiTextureType_SPECULAR, 0, &path, NULL, NULL, NULL, NULL, NULL) == aiReturn_SUCCESS)
            {

                std::string fullPath = path.data;
                printf("specular path: %s\n", fullPath.c_str());
                spec = resourceManager.LoadTextureFromFile(fullPath);
            }
        }
        aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shine);
        Material mat = {
            .tint = glm::vec3(1),
            .diffuseMap = diff,
            .specularMap = spec,
            .shininess = shine};

        std::vector<Vertex> verts;
        verts.reserve(mesh->mNumVertices);

        for (u32 vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++)
        {
            aiVector3d pos = mesh->mVertices[vertexIndex];
            aiVector3d normal = mesh->mNormals[vertexIndex];

            Vertex vert = {
                .position = glm::vec3(pos.x, pos.y, pos.z),
                .normal = glm::vec3(normal.x, normal.y, normal.z),
                .uv = glm::vec3(1, 1, 0),
                .color = glm::vec3(1)};
            verts.push_back(vert);
            printf("vert: %d | x: %f | y: %f | z: %f\n", vertexIndex, pos.x, pos.y, pos.z);
        }

        std::vector<u32> indices;
        indices.reserve(mesh->mNumFaces * 3);
        for (u32 faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++)
        {
            aiFace face = mesh->mFaces[faceIndex];
            u32 tri1 = face.mIndices[0];
            u32 tri2 = face.mIndices[1];
            u32 tri3 = face.mIndices[2];
            indices.push_back(tri1);
            indices.push_back(tri2);
            indices.push_back(tri3);
            printf("indi: %d | 1: %u | 2: %u | 3: %u\n", faceIndex, tri1, tri2, tri3);
        }

        Mesh mMesh = {
            .vertices = verts,
            .material = mat,
            .indices = indices,
        };

        meshes.push_back(mMesh);
    }

    Model *model = new Model{
        .meshes = meshes};

    aiReleaseImport(scene);
    return model;
}