#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <common/File/File.h>
#include <common/stb_image.h>
#include <core/assert.h>
#include <core/graphics/model.h>
#include <core/graphics/renderer/renderer.h>
#include <core/graphics/renderer/shader.h>
#include <core/resources/resource_system.h>
#include <core/utils/uuid/uuid.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace ResourceSystem {
// NOTE: based on https://github.com/ekardnam/Newtonic/
std::unordered_map<std::string, Handle> m_nameHandleMap = {};
std::unordered_map<Handle, Resource> m_resources = {};
std::vector<ResourceDescriptor> m_resourceDescriptors = {};
// todo: Create a Emplace function. For creating resources handles from already
// loaded resources.

std::string GetResourceFolderPath();
void LoadResource(Handle h);
void LoadTextureFromFile(ResourceDescriptor descriptor, Handle h);
void LoadShadersFromFile(ResourceDescriptor descriptor, Handle h);
void LoadModelFromFile(ResourceDescriptor descriptor, Handle h);
void ProcessMesh(aiMesh *mesh, glm::mat4 meshTransformation);
void TraverseNodes(aiNode *node, glm::mat4 transform);
void GetTotalMeshes(aiNode *node);

Handle GetDefaultTexture() {
  return m_nameHandleMap["defaults/textures/white"];
};
Handle GetDefaultModel() { return m_nameHandleMap["defaults/textures/white"]; };
Handle GetDefaultShader() {
  return m_nameHandleMap["defaults/textures/white"];
};

// todo make base resource folder changeable
std::string GetResourceFolderPath() { return "../resources/"; };

// todo bad solution - assumes all paths are / (as they should really, fuck
// windows)
std::string GetFileFolderPath(std::string file) {
  u64 slash = file.rfind("/");
  if (std::string::npos != slash) {
    return file.substr(0, slash);
  }
  return file;
};

Handle currentHandle = 0;

Handle PrepareResource(const ResourceDescriptor resourceDescriptor) {
  Handle h = currentHandle;
  // NOTE: Assumes that names are unique...
  if (m_nameHandleMap.find(resourceDescriptor.name) != m_nameHandleMap.end()) {
    return m_nameHandleMap[resourceDescriptor.name];
  }
  m_resourceDescriptors.push_back(resourceDescriptor);
  m_nameHandleMap[resourceDescriptor.name] = h;
  currentHandle++;
  return h;
}

// todo reference counting?
Resource *GetResource(Handle h) {
  // ASSERT(h < currentHandle);
  if (m_resources.find(h) == m_resources.end())
    LoadResource(h);
  return &m_resources[h];
}
Resource *GetResource(std::string rName) {
  // ASSERT(m_nameHandleMap.find(rName) != m_nameHandleMap.end())
  return GetResource(m_nameHandleMap[rName]);
}

// NOTE: basically a factory?
void LoadResource(Handle handle) {
  ResourceDescriptor descriptor = m_resourceDescriptors[handle];
  switch (descriptor.type) {
  case RESOURCE_MATERIAL:
    return;
  case RESOURCE_MESH:
    return;
  case RESOURCE_MODEL:
    LoadModelFromFile(descriptor, handle);
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

void LoadTextureFromFile(ResourceDescriptor descriptor, Handle h) {

  Texture *texture = new Texture;
  std::string filepath = GetResourceFolderPath().append(descriptor.path);

  byte *data = stbi_load(filepath.c_str(), &texture->width, &texture->height,
                         (i32 *)&texture->nrChannels, STBI_rgb_alpha);
  if (!data) {
    const char *failureReason = stbi_failure_reason();
    printf("error loading texture: %s\n", failureReason);
    exit(1);
  }

  texture->data = data;

  Renderer::GetInstance().CreateGPUTexture(*texture);

  printf("texture: %s loaded successfully, id: %d\n", descriptor.path.c_str(),
         texture->id);

  m_resources[h] = {.data = texture};
};

void LoadShadersFromFile(ResourceDescriptor descriptor, Handle h) {
  ShaderProgram *shaderProgram = new ShaderProgram();
  std::string vertexShaderFile = descriptor.path;
  vertexShaderFile.append("_vert.vs");
  std::string fragmentShaderFile = descriptor.path;
  fragmentShaderFile.append("_frag.fs");

  // todo Make good resource system file reading
  char *vertexShader = FileUtils::readFile(vertexShaderFile.c_str());
  char *fragmentShader = FileUtils::readFile(fragmentShaderFile.c_str());

  Renderer::GetInstance().CreateShaderProgram(vertexShader, fragmentShader,
                                              *shaderProgram);
  free(vertexShader);
  free(fragmentShader);
  m_resources[h] = {.data = shaderProgram};
}

void LoadHardcodedCubeMeshTextured(Handle h);
void LoadHardcodedCubeMesh(Handle h);

void Init() {
  stbi_set_flip_vertically_on_load(1);
  Handle defaultWhite =
      PrepareResource({.path = "defaults/textures/white_1x1.jpg",
                       .name = "defaults/textures/white",
                       .type = RESOURCE_TEXTURE});
  Handle defaultLit = PrepareResource({.path = "defaults/shaders/lit",
                                       .name = "defaults/shaders/lit",
                                       .type = RESOURCE_SHADER});
  Handle defaultUnlit = PrepareResource({.path = "defaults/shaders/unlit",
                                         .name = "defaults/shaders/unlit",
                                         .type = RESOURCE_SHADER});
  LoadResource(defaultWhite);
  LoadResource(defaultLit);
  LoadResource(defaultUnlit);

  // Hardcoded loading
  Handle defaultCube = PrepareResource({.path = "hardcoded/cube",
                                        .name = "defaults/model/cube_tex",
                                        .type = RESOURCE_MODEL});
  Handle defaultUntextured = PrepareResource({.path = "hardcoded/cube",
                                              .name = "defaults/model/cube",
                                              .type = RESOURCE_MODEL});
  LoadHardcodedCubeMeshTextured(defaultCube);
  LoadHardcodedCubeMesh(defaultUntextured);
};
// todo Cleanup function

void LoadHardcodedCubeMeshTextured(Handle h) {
  const float cube_vert_pos[108] = {
      -0.5f, -0.5f, -0.5f, //
      0.5f,  -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, -0.5f,
      0.5f,  -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f,
      0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,
      -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,
      0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f,
      0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,  -0.5f,
      -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f,
      0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f,
      0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f,
      0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f};
  const float cube_vert_uvs[72] = {
      0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
      1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
      0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
  const float cube_vert_normals[108] = {
      0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f,
      0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f,

      0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
      0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,

      -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,
      -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,

      1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

      0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,
      0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,

      0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
      0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f};

  Graphics::Mesh cubeMesh;
  Handle diffuse = PrepareResource({.path = "container.png",
                                    .name = "container_diffuse",
                                    .type = RESOURCE_TEXTURE});
  Handle specular = PrepareResource({.path = "container_specular.png",
                                     .name = "container_specular",
                                     .type = RESOURCE_TEXTURE});

  std::vector<Graphics::Vertex> vertices = std::vector<Graphics::Vertex>();
  vertices.reserve(36);
  for (u32 i = 0; i < 36; i++) {
    Graphics::Vertex vert;
    vert.position = glm::vec3(cube_vert_pos[i * 3], cube_vert_pos[i * 3 + 1],
                              cube_vert_pos[i * 3 + 2]);
    vert.normal =
        glm::vec3(cube_vert_normals[i * 3], cube_vert_normals[i * 3 + 1],
                  cube_vert_normals[i * 3 + 2]);
    vert.uv = glm::vec3(cube_vert_uvs[i * 2], cube_vert_uvs[i * 2 + 1], 1);
    vertices.push_back(vert);
  }

  cubeMesh.vertices = vertices;
  Handle mat = PrepareResource({.path = "hardcoded/textured_cube_mat",
                                .name = "hardcoded/textured_cube_mat",
                                .type = RESOURCE_MATERIAL});
  Material *material = new Material{.shader = "defaults/shaders/lit",
                                    .tint = glm::vec3(1),
                                    .diffuseMap = diffuse,
                                    .specularMap = specular,
                                    .shininess = 64};
  m_resources[mat] = {.data = material};
  cubeMesh.material = mat;

  std::vector<Graphics::Mesh> meshes = std::vector<Graphics::Mesh>();
  meshes.push_back(cubeMesh);
  Graphics::Model *model = new Graphics::Model{.meshes = meshes};
  m_resources[h] = {.data = model};
}
void LoadHardcodedCubeMesh(Handle h) {
  const float cube_vert_pos[108] = {
      -0.5f, -0.5f, -0.5f, //
      0.5f,  -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, -0.5f,
      0.5f,  -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f,
      0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,
      -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,
      0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f,
      0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,  -0.5f,
      -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f,
      0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f,
      0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f,
      0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f};
  const float cube_vert_uvs[72] = {
      0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
      1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
      0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
  const float cube_vert_normals[108] = {
      0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f,
      0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f,

      0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
      0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,

      -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,
      -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,

      1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

      0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,
      0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,

      0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
      0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f};

  Graphics::Mesh cubeMesh;
  Handle diffuse = m_nameHandleMap["defaults/textures/white"];
  Handle specular = m_nameHandleMap["defaults/textures/white"];

  std::vector<Graphics::Vertex> vertices = std::vector<Graphics::Vertex>();
  vertices.reserve(36);

  for (u32 i = 0; i < 36; i++) {
    Graphics::Vertex vert;
    vert.position = glm::vec3(cube_vert_pos[i * 3], cube_vert_pos[i * 3 + 1],
                              cube_vert_pos[i * 3 + 2]);
    vert.normal =
        glm::vec3(cube_vert_normals[i * 3], cube_vert_normals[i * 3 + 1],
                  cube_vert_normals[i * 3 + 2]);
    vert.uv = glm::vec3(cube_vert_uvs[i * 2], cube_vert_uvs[i * 2 + 1], 1);
    vertices.push_back(vert);
  }

  cubeMesh.vertices = vertices;
  Handle mat = PrepareResource({.path = "hardcoded/untextured_cube_mat",
                                .name = "hardcoded/untextured_cube_mat",
                                .type = RESOURCE_MATERIAL});
  Material *material = new Material{.shader = "defaults/shaders/unlit",
                                    .tint = glm::vec3(1),
                                    .diffuseMap = diffuse,
                                    .specularMap = specular};
  m_resources[mat] = {.data = material};
  cubeMesh.material = mat;

  std::vector<Graphics::Mesh> meshes = std::vector<Graphics::Mesh>();
  meshes.push_back(cubeMesh);
  Graphics::Model *model = new Graphics::Model{.meshes = meshes};

  m_resources[h] = {.data = model};
}

//----------
// NOTE: MODEL LOADING
const aiScene *modelScene = nullptr;
std::string modelFolder;
u32 total = 0;
std::vector<Graphics::Mesh> meshes;

glm::mat4 identity = glm::mat4(1);
glm::mat4 ConvertMatrix(const aiMatrix4x4 &from) {
  return glm::transpose(glm::make_mat4(&from.a1));
}

void GetTotalMeshes(aiNode *node) {
  total += node->mNumMeshes;
  for (u32 child = 0; child < node->mNumChildren; child++) {
    GetTotalMeshes(node->mChildren[child]);
  }
};

void ProcessMesh(aiMesh *mesh, glm::mat4 meshTransformation) {
  aiMaterial *material = modelScene->mMaterials[mesh->mMaterialIndex];
  f32 shine;

  // todo: CAN BREAK IF NOT LOADED, SHOULD CALL PREPARERESOURCE
  // TODO: resource system probably should stop loading only on GetResource.
  Handle diffuse = GetDefaultTexture();
  Handle specular = diffuse;

  // NOTE: Handle only one texture type per material
  if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
    aiString path;
    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) ==
        aiReturn_SUCCESS) {
      std::string fullPath = modelFolder + path.data;
      printf("diffuse path: %s\n", fullPath.c_str());
      diffuse = ResourceSystem::PrepareResource(
          {.path = fullPath, .name = fullPath, .type = RESOURCE_TEXTURE});
    }
  }
  if (material->GetTextureCount(aiTextureType_SPECULAR) > 0) {
    aiString path;
    if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) ==
        aiReturn_SUCCESS) {

      std::string fullPath = modelFolder + path.data;
      printf("specular path: %s\n", fullPath.c_str());
      specular = ResourceSystem::PrepareResource(
          {.path = fullPath, .name = fullPath, .type = RESOURCE_TEXTURE});
    }
  }
  // NOTE: Idea to handle multiple textures, however my Material cant handle
  // this yet, nor i care to handle now;
  //   for (u32 diffuseTexture = 0; diffuseTexture <
  //   material->GetTextureCount(aiTextureType_DIFFUSE); diffuseTexture++)
  //  {
  //      aiString path;
  //      if (material->GetTexture(aiTextureType_DIFFUSE, diffuseTexture, &path)
  //      == aiReturn_SUCCESS)
  //      {
  //          std::string fullPath = path.data;
  //          printf("diffuse path: %s\n", fullPath.c_str());
  //          diffuse = ResourceSystem::PrepareResource({.path = fullPath, .name
  //          = fullPath, .type = RESOURCE_TEXTURE});
  //      }
  //  }
  aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shine);
  // todo Resource System material loading
  Material *mat = new Material{.tint = glm::vec3(1),
                               .diffuseMap = diffuse,
                               .specularMap = specular,
                               .shininess = shine};
  std::string uuid = Random::GenerateStringUUID();
  Handle materialHandle = ResourceSystem::PrepareResource(
      {.path = uuid, .name = uuid, .type = RESOURCE_MATERIAL});
  m_resources[materialHandle] = {.data = mat};
  std::vector<Graphics::Vertex> vertices;
  vertices.reserve(mesh->mNumVertices);

  for (u32 vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++) {
    aiVector3d pos = mesh->mVertices[vertexIndex];
    aiVector3d normal = mesh->mNormals[vertexIndex];
    aiVector3D uv = aiVector3D(1);
    if (mesh->HasTextureCoords(0)) {
      uv = mesh->mTextureCoords[0][vertexIndex];
    }
    Graphics::Vertex vertex = {.position = glm::vec3(pos.x, pos.y, pos.z),
                               .normal =
                                   glm::vec3(normal.x, normal.y, normal.z),
                               .uv = glm::vec3(uv.x, uv.y, 0),
                               .color = glm::vec3(1)};
    vertices.push_back(vertex);
  }

  std::vector<u32> indices;
  indices.reserve(mesh->mNumFaces * 3);
  for (u32 faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++) {
    const aiFace &face = mesh->mFaces[faceIndex];
    u32 tri1 = face.mIndices[0];
    u32 tri2 = face.mIndices[1];
    u32 tri3 = face.mIndices[2];
    indices.push_back(tri1);
    indices.push_back(tri2);
    indices.push_back(tri3);
    printf("triangulo %d: | 1: %u | 2: %u | 3: %u\n", faceIndex, tri1, tri2,
           tri3);
  }

  Graphics::Mesh mMesh = {.vertices = vertices,
                          .indices = indices,
                          .material = materialHandle,
                          .localTransform = meshTransformation};

  meshes.push_back(mMesh);
}
void TraverseNodes(aiNode *node, glm::mat4 transform) {
  glm::mat4 nodeTransformation = ConvertMatrix(node->mTransformation);
  glm::mat4 worldTransformation = transform * nodeTransformation;
  for (u32 numMesh = 0; numMesh < node->mNumMeshes; numMesh++) {
    aiMesh *mesh = modelScene->mMeshes[node->mMeshes[numMesh]];
    ProcessMesh(mesh, worldTransformation);
  }
  for (u32 child = 0; child < node->mNumChildren; child++) {
    TraverseNodes(node->mChildren[child], transform);
  }
};

void LoadModelFromFile(ResourceDescriptor descriptor, Handle h) {

  modelFolder = GetFileFolderPath(descriptor.path);
  std::string newPath =
      ResourceSystem::GetResourceFolderPath().append(descriptor.path);
  Assimp::Importer importer;
  const aiScene *scene =
      importer.ReadFile(newPath.c_str(), aiProcessPreset_TargetRealtime_Fast);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    const char *error = importer.GetErrorString();
    printf("error loading model kaboom %s\n", error);
    exit(1);
  }

  modelScene = scene;

  GetTotalMeshes(scene->mRootNode);
  meshes.reserve(total);
  TraverseNodes(scene->mRootNode, identity);

  Graphics::Model *model = new Graphics::Model{.meshes = meshes};

  m_resources[h] = {.data = model};
}
//----------
} // namespace ResourceSystem
