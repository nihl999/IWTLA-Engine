#include <GLAD/glad.h>
#include <stdio.h>
#include <stdlib.h>

namespace Shader
{
    GLuint createShader(const char *shaderSrc, GLenum shaderType)
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

    GLuint createProgram(GLuint vertexShaderId, GLuint fragmentShaderId, bool deleteShaders)
    {
        GLint result = GL_FALSE;
        int infoLogLength;

        GLuint programId = glCreateProgram();
        glAttachShader(programId, vertexShaderId);
        glAttachShader(programId, fragmentShaderId);
        glLinkProgram(programId);

        glGetProgramiv(programId, GL_LINK_STATUS, &result);
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
        printf("shader program link result: %d", result);
        if (infoLogLength > 0)
        {
            char programErrorMessage[infoLogLength + 1];
            glGetProgramInfoLog(programId, infoLogLength, NULL, &programErrorMessage[0]);
            printf("%s\n", programErrorMessage);
            exit(1);
        }

        glDetachShader(programId, vertexShaderId);
        glDetachShader(programId, fragmentShaderId);

        if (deleteShaders)
        {
            glDeleteShader(vertexShaderId);
            glDeleteShader(fragmentShaderId);
        }
        return programId;
    }
}