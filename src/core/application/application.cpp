#define GLFW_INCLUDE_NONE
#include <core/application/application.h>
#include <stdio.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include "Input.h"
#include <core/world/scene.h>
#include <core/window/window.h>
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

Application::Application() : window(Window(1280, 720)), renderer(Renderer::GetInstance())
{
    resourceManager = &ResourceManager::GetInstance();
    currentScene = Scene(Camera(), {new CubeModel(glm::vec3(0, 0, -5)), new CubeModel()});
    startTime = glfwGetTime();

    ShaderProgram shaderProgram = resourceManager->LoadShadersFromFile("vertex.shader", "fragment.shader");

    renderer.BindShaderProgram(shaderProgram);
    glUniform1f(2, currentScene.ambientLight.intensity);
}

void Application::Update()
{
    if (window.ShouldClose())
    {
        shouldRun = false;
    }
    window.PoolEvents();
    deltaTime = glfwGetTime() - lastFrameTime;
    lastFrameTime = glfwGetTime();

    window.NewFrame();
    renderer.NewFrame();

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
    // todo no
    glUniform3fv(5,
                 1,
                 &currentScene.camera.position[0]);
    for (Entity *entity : currentScene.entities)
    {
        HasMesh *meshInfo = dynamic_cast<HasMesh *>(entity);
        if (meshInfo == nullptr)
            continue;
        glUniformMatrix4fv(1,
                           1,
                           GL_FALSE,
                           &entity->modelMatrix[0][0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(MeshVertex) * meshInfo->mesh->verticeCount, &meshInfo->mesh->vertices[0], GL_STATIC_DRAW);
        // todo for the love of god, create a method for getting map itens
        Texture tex;
        try
        {
            tex = resourceManager->textures.at(meshInfo->mesh->textureName);
        }
        catch (const std::out_of_range &oor)
        {
            // todo hate try catches, find a way
            printf("\ntexture doesnt exist on resource manager, exploding\n");
            exit(1);
        }
        if (tex.id != InvalidTexId)
        {
            glBindTexture(GL_TEXTURE_2D, tex.id);
        }
        glDrawArrays(GL_TRIANGLES, 0, sizeof(MeshVertex) * meshInfo->mesh->verticeCount);
    }

    DrawApplicationPropertiesDebug();
    currentScene.camera.DrawCameraPropertiesDebug();

    renderer.EndFrame();

    window.SwapBuffers();
    // todo dont realy want to poll mouse position, but it will be what it will be

    Input::mouseDeltaX = 0;
    Input::mouseDeltaY = 0;
}
