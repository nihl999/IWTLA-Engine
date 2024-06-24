#pragma once
#include <core/defines.h>
#include <core/graphics/mesh.h>
#include <core/graphics/renderer/shader.h>
#include <glm/glm.hpp>

// todo organize and padronize - really, read the GEA book
class Renderer
{
public:
    static Renderer &GetInstance();
    void NewFrame();
    void EndFrame();
    void RenderMesh(Mesh mesh);

    // resources side
    void CreateGPUTexture(Texture &texture);
    void CreateShaderProgram(char *vrtSrc, char *frgSrc, ShaderProgram &outShaderProgram);

    // shader side
    void BindShaderProgram(ShaderProgram shader);
    bool UniformFMat4(std::string variableName, const glm::mat4 &value);
    bool UniformFVec3(std::string variableName, const glm::vec3 &value);
    bool UniformF1(std::string variableName, const f32 value);
    bool UniformI1(std::string variableName, const i32 value);

    bool UniformMaterial(Material &material);

private:
    static Renderer *instance;
    ShaderProgram currentShaderProgram;
    Renderer();
};