#pragma once
#include <core/defines.h>
#include <core/graphics/mesh.h>
#include <core/renderer/shader.h>
#include <glm/glm.hpp>

class Renderer
{
public:
    void BindShaderProgram(ShaderProgram shader);
    void CreateGPUTexture(Texture &texture);
    i32 CreateShaderProgram(char *vrtSrc, char *frgSrc);
    void NewFrame();
    void EndFrame();
    static Renderer &GetInstance();

private:
    static Renderer *instance;
    Renderer();
};