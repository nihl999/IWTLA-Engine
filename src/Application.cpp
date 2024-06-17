#define GLFW_INCLUDE_NONE
#include "Application.h"
#include <stdio.h>
#include <GLAD/glad.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include "Input.h"
#include <core/world/scene.h>
#include "Shader.h"
#include <common/File/File.h>
#include <core/resources/resource_manager.h>

void Application::DrawApplicationPropertiesDebug()
{
    ImGui::Begin("Application Properties");
    ImGui::Checkbox("Capture Mouse", &Input::capturingMouse);
    // todo Should be on some inputs tab maybe? or application handles... anyway
    ImGui::SliderFloat("Mouse Sensitivity", &Input::sensitivity, 0.01f, 3.0f, "%.2f");
    ImGui::Text("Mouse X: %.2f", Input::mousePositionX);
    ImGui::Text("Mouse Y: %.2f", Input::mousePositionY);
    ImGui::Text("Mouse X Offset: %.3f", Input::mouseDeltaX);
    ImGui::Text("Mouse Y Offset: %.3f", Input::mouseDeltaY);
    ImGui::End();
    currentScene.DrawScenePropertiesDebug();
}

Application::Application() : currentScene(Scene(Camera(), {new CubeModel(glm::vec3(0, 0, -5)), new CubeModel()}))
{
    if (!glfwInit())
    {
        printf("Error initializing GLFW library!");
        exit(1);
    }

    window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        printf("Error initializing window with GLFW!");
        exit(1);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwSetWindowAspectRatio(window, widthAspectRatio, heightAspectRatio);
    glfwSetKeyCallback(window, &Input::KeyCallback);
    glfwSetCursorPosCallback(window, &Input::MouseCallback);
    glfwSetWindowSizeCallback(window, &Application::WindowSizeCallback);

    gladLoadGL();
    shouldRun = true;

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
    startTime = glfwGetTime();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // TODO SHOULD BE ON RENDERER WITH EVERYTHING ELSE, SPECIALLY THIS
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
        sizeof(Graphics::MeshVertex),
        (void *)offsetof(Graphics::MeshVertex, position));
    printf("pos offsetof: %d", offsetof(Graphics::MeshVertex, position));
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Graphics::MeshVertex),
        (void *)offsetof(Graphics::MeshVertex, normal));
    printf("normal offsetof: %d", offsetof(Graphics::MeshVertex, normal));
    glVertexAttribPointer(
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Graphics::MeshVertex),
        (void *)offsetof(Graphics::MeshVertex, texCoord));
    printf("texCoord offsetof: %d", offsetof(Graphics::MeshVertex, texCoord));

    char *vertexShader = FileUtils::readFile("vertex.shader");
    char *fragmentShader = FileUtils::readFile("fragment.shader");

    GLuint vertexShaderId = Shader::createShader(vertexShader, GL_VERTEX_SHADER);
    free(vertexShader);
    GLuint fragmentShaderId = Shader::createShader(fragmentShader, GL_FRAGMENT_SHADER);
    free(fragmentShader);

    GLuint programId = Shader::createProgram(vertexShaderId, fragmentShaderId, true);
    glUseProgram(programId);

    //--
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // todo refactor - When renderer class, ResourceManager call renderer to generate tex in GPU and dont do it here...
    for (Entity *entity : currentScene.entities)
    {
        HasMesh *meshInfo = dynamic_cast<HasMesh *>(entity);
        if (meshInfo == nullptr)
            continue;
        Graphics::Texture tex;
        try
        {
            tex = ResourceManager::textures.at(meshInfo->mesh->textureName);
        }
        catch (const std::out_of_range &oor)
        {
            // todo hate try catches, find a way
            printf("\ntexture doesnt exist on resource manager, exploding\n");
            exit(1);
        }
        if (tex.id != Graphics::invalidTexId)
            continue;
        u32 texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        tex.id = texture;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex.width, tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex.data.get()[0]);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    glUniform1f(2, currentScene.ambientLight.intensity);
}

void Application::WindowSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// todo for now, application is tied with renderer, separate
void Application::Update()
{
    if (glfwWindowShouldClose(window))
    {
        shouldRun = false;
    }
    /* Poll for and process events */
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    deltaTime = glfwGetTime() - lastFrameTime;
    lastFrameTime = glfwGetTime();

    currentScene.camera.move(glm::vec3(Input::axis["Horizontal"].currentAxisValue * deltaTime, Input::axis["Fly"].currentAxisValue * deltaTime, Input::axis["Vertical"].currentAxisValue * deltaTime));
    currentScene.camera.updateYawPitch(Input::mouseDeltaX, Input::mouseDeltaY);

    currentScene.camera.update();
    currentScene.Update();
    glm::mat4 viewProj = currentScene.camera.getViewMatrix();

    glUniformMatrix4fv(0,
                       1,
                       false,
                       &viewProj[0][0]);

    glClearColor(0.2f, 0.2f, 0.2f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // todo better shader uniform handling
    glUniform1f(2, currentScene.ambientLight.intensity);
    glUniform3fv(3,
                 1,
                 &currentScene.pointLights[0].position[0]);
    glUniform3fv(4,
                 1,
                 &currentScene.pointLights[0].color[0]);
    for (Entity *entity : currentScene.entities)
    {
        HasMesh *meshInfo = dynamic_cast<HasMesh *>(entity);
        if (meshInfo == nullptr)
            continue;
        glUniformMatrix4fv(1,
                           1,
                           GL_FALSE,
                           &entity->modelMatrix[0][0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Graphics::MeshVertex) * meshInfo->mesh->verticeCount, &meshInfo->mesh->vertices[0], GL_STATIC_DRAW);
        // todo for the love of god, create a method for getting map itens
        Graphics::Texture tex;
        try
        {
            tex = ResourceManager::textures.at(meshInfo->mesh->textureName);
        }
        catch (const std::out_of_range &oor)
        {
            // todo hate try catches, find a way
            printf("\ntexture doesnt exist on resource manager, exploding\n");
            exit(1);
        }
        if (tex.id != Graphics::invalidTexId)
        {
            glBindTexture(GL_TEXTURE_2D, tex.id);
        }
        glDrawArrays(GL_TRIANGLES, 0, sizeof(Graphics::MeshVertex) * meshInfo->mesh->verticeCount);
    }
    /* Render here */

    DrawApplicationPropertiesDebug();
    currentScene.camera.DrawCameraPropertiesDebug();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
    // todo dont realy want to poll mouse position, but it will be what it will be

    Input::mouseDeltaX = 0;
    Input::mouseDeltaY = 0;
}

void Application::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}