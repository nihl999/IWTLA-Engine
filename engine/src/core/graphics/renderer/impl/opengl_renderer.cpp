#include <GLAD/glad.h>
#include <core/graphics/material.h>
#include <core/graphics/renderer/renderer.h>
#include <core/graphics/texture.h>
#include <core/resources/resource_system.h>
#include <imgui/backends/imgui_impl_opengl3.h>

i32 intern_CreateShader(const char *shaderSrc, GLenum shaderType)
{
    GLint result = GL_FALSE;
    int infoLogLength;

    GLuint shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &shaderSrc, NULL);
    glCompileShader(shaderId);

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        char shaderErrorMessage[infoLogLength + 1];
        glGetShaderInfoLog(shaderId, infoLogLength, NULL, &shaderErrorMessage[0]);
        printf("%s\n", shaderErrorMessage);
        exit(1);
    }
    return shaderId;
}

Renderer *Renderer::instance = nullptr;

Renderer::Renderer() : currentShaderProgram(ShaderProgram())
{
    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    GLuint vertexBufferId;
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

    GLuint elementBufferId;
    glGenBuffers(1, &elementBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferId);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Graphics::Vertex),
        (void *)offsetof(Graphics::Vertex, position));
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Graphics::Vertex),
        (void *)offsetof(Graphics::Vertex, normal));
    glVertexAttribPointer(
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Graphics::Vertex),
        (void *)offsetof(Graphics::Vertex, uv));
    glVertexAttribPointer(
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Graphics::Vertex),
        (void *)offsetof(Graphics::Vertex, color));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
}

Renderer &Renderer::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new Renderer();
    }
    return *instance;
}

void Renderer::NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    glClearColor(0.2f, 0.2f, 0.2f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::RenderMesh(Graphics::Mesh mesh)
{
    ResourceSystem::Resource *materialResource = ResourceSystem::GetResource(mesh.material);
    if (materialResource == nullptr)
    {
        printf("Material %d doesn't exist!\n", mesh.material);
        materialResource = ResourceSystem::GetResource("defaults/material/unlit");
    }
    Material *material = (Material *)materialResource->data;
    UniformMaterial(*material);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Graphics::Vertex) * mesh.vertices.size(), &mesh.vertices[0], GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(Graphics::Vertex) * mesh.vertices.size());
};

void Renderer::RenderMeshIndexed(Graphics::Mesh mesh)
{
    ResourceSystem::Resource *materialResource = ResourceSystem::GetResource(mesh.material);
    if (materialResource == nullptr)
    {
        printf("Material %d doesn't exist!\n", mesh.material);
        materialResource = ResourceSystem::GetResource("defaults/material/unlit");
    }
    Material *material = (Material *)materialResource->data;
    UniformMaterial(*material);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Graphics::Vertex) * mesh.vertices.size(), &mesh.vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * mesh.indices.size(), &mesh.indices[0], GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, NULL);
};

void Renderer::BindShaderProgram(ShaderProgram program)
{
    glUseProgram(program.id);
    currentShaderProgram = program;
    return;
}

void Renderer::CreateGPUTexture(Texture &texture)
{
    u32 tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    texture.id = tex;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &texture.data[0]);
    glGenerateMipmap(GL_TEXTURE_2D);
    return;
}

void Renderer::CreateShaderProgram(char *vrtSrc, char *frgSrc, ShaderProgram &outShaderProgram)
{
    i32 vertex = intern_CreateShader(vrtSrc, GL_VERTEX_SHADER);
    i32 fragment = intern_CreateShader(frgSrc, GL_FRAGMENT_SHADER);

    i32 result = GL_FALSE;
    int infoLogLength;

    i32 programId = glCreateProgram();
    glAttachShader(programId, vertex);
    glAttachShader(programId, fragment);
    glLinkProgram(programId);

    glGetProgramiv(programId, GL_LINK_STATUS, &result);
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
    printf("shader program link result: %d\n", result);
    if (infoLogLength > 0)
    {
        char programErrorMessage[infoLogLength + 1];
        glGetProgramInfoLog(programId, infoLogLength, NULL, &programErrorMessage[0]);
        printf("%s\n", programErrorMessage);
        exit(1);
    }

    glDetachShader(programId, vertex);
    glDetachShader(programId, fragment);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    outShaderProgram.id = programId;
    return;
}

bool Renderer::UniformFMat4(std::string variableName, const glm::mat4 &value)
{
    i32 uniformLocation = glGetUniformLocation(currentShaderProgram.id, variableName.c_str());
    glUniformMatrix4fv(uniformLocation,
                       1,
                       false,
                       &value[0][0]);
    return true;
}
bool Renderer::UniformFVec3(std::string variableName, const glm::vec3 &value)
{
    i32 uniformLocation = glGetUniformLocation(currentShaderProgram.id, variableName.c_str());
    glUniform3fv(uniformLocation,
                 1,
                 &value[0]);
    return true;
}
bool Renderer::UniformF1(std::string variableName, const f32 value)
{
    i32 uniformLocation = glGetUniformLocation(currentShaderProgram.id, variableName.c_str());
    glUniform1f(uniformLocation,
                value);
    return true;
}
bool Renderer::UniformD1(std::string variableName, const f64 value) {
  i32 uniformLocation =
      glGetUniformLocation(currentShaderProgram.id, variableName.c_str());
  glUniform1d(uniformLocation, value);
  return true;
}
bool Renderer::UniformI1(std::string variableName, const i32 value)
{
    i32 uniformLocation = glGetUniformLocation(currentShaderProgram.id, variableName.c_str());
    glUniform1i(uniformLocation,
                value);
    return true;
}
bool Renderer::UniformU1(std::string variableName, const u32 value) {
  i32 uniformLocation =
      glGetUniformLocation(currentShaderProgram.id, variableName.c_str());
  glUniform1ui(uniformLocation, value);
  return true;
}
bool Renderer::UniformMaterial(Material &material)
{
    ResourceSystem::Resource *diffuseResource = ResourceSystem::GetResource(material.diffuseMap);
    if (diffuseResource == nullptr)
    {
      printf("kabooom diffuse not found %d\n", material.diffuseMap);
      exit(1);
    }
    ResourceSystem::Resource *specularResource = ResourceSystem::GetResource(material.specularMap);
    if (!specularResource)
    {
      printf("kabooom specular not found %d\n", material.diffuseMap);
      exit(1);
    }
    Texture *diffuseTexture = (Texture *)diffuseResource->data;
    Texture *specularTexture = (Texture *)specularResource->data;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture->id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularTexture->id);
    UniformI1("material.diffuse",
              0);
    UniformI1("material.specular",
              1);
    UniformF1("material.shininess",
              material.shininess);
    return true;
}