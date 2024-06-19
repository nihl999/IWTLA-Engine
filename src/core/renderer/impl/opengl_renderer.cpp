#include <core/renderer/renderer.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <GLAD/glad.h>

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

Renderer::Renderer()
{
    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    GLuint vertexBufferId;
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(MeshVertex),
        (void *)offsetof(MeshVertex, position));
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(MeshVertex),
        (void *)offsetof(MeshVertex, normal));
    printf("normal offsetof: %d", offsetof(MeshVertex, normal));
    glVertexAttribPointer(
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(MeshVertex),
        (void *)offsetof(MeshVertex, texCoord));

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
}

void Renderer::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

i32 Renderer::CreateShaderProgram(char *vrtSrc, char *frgSrc)
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
    printf("shader program link result: %d", result);
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
    return programId;
}

void Renderer::BindShaderProgram(ShaderProgram program)
{
    glUseProgram(program.id);
}

void Renderer::CreateGPUTexture(Texture &texture)
{
    u32 tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    texture.id = tex;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.data.get()[0]);
    glGenerateMipmap(GL_TEXTURE_2D);
    return;
}