#ifndef SHADER_IMPL
#define SHADER_IMPL

#include <GLAD/glad.h>

namespace Shader
{
    GLuint createShader(const char *shaderSrc, GLenum shaderType);
    GLuint createProgram(GLuint vertexShaderId, GLuint fragmentShaderId, bool deleteShaders);
}

#endif