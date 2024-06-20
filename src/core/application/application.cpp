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
#include <core/graphics/utils/light_model.h>
#include <core/graphics/utils/cube_model.h>

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
    currentScene = Scene(Camera(), {new CubeModel(glm::vec3(0, 0, -5)), new CubeModel(), new LightModel(currentScene.pointLights[0].position)});
    startTime = glfwGetTime();

    // todo nope
    ShaderProgram litShaderProgram = resourceManager->LoadShadersFromFile("lit", "lit_vertex.vs", "lit_fragment.fs");
    ShaderProgram unlitShaderProgram = resourceManager->LoadShadersFromFile("unlit", "unlit_vertex.vs", "unlit_fragment.fs");

    renderer.BindShaderProgram(litShaderProgram);
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

    currentScene.entities[2]->position = currentScene.pointLights[0].position;

    for (Entity *entity : currentScene.entities)
    {
        HasMesh *meshInfo = dynamic_cast<HasMesh *>(entity);
        if (meshInfo == nullptr)
            continue;

        // todo learn UBOs
        std::optional<ShaderProgram> shader = resourceManager->GetShader(meshInfo->mesh->material.shader);

        if (!shader)
        {
            printf("kabooom shader not found %s\n", meshInfo->mesh->material.shader);
            exit(1);
        }

        renderer.BindShaderProgram(shader.value());
        renderer.UniformFMat4("view_projection_matrix", currentScene.camera.getViewMatrix());

        renderer.UniformF1("ambient_light_force", currentScene.pointLights[0].intensity);
        renderer.UniformFVec3("point_light_position",
                              currentScene.pointLights[0].position);
        renderer.UniformFVec3("point_light_color",
                              currentScene.pointLights[0].color);
        renderer.UniformFVec3("cam_pos",
                              currentScene.camera.position);
        renderer.UniformFMat4("model_matrix",
                              entity->modelMatrix);

        renderer.RenderMesh(*meshInfo->mesh);
    }

    DrawApplicationPropertiesDebug();
    currentScene.camera.DrawCameraPropertiesDebug();

    renderer.EndFrame();

    window.SwapBuffers();
    // todo dont realy want to poll mouse position, but it will be what it will be

    Input::mouseDeltaX = 0;
    Input::mouseDeltaY = 0;
}
