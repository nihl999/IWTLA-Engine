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
#include <core/resources/resource_system.h>
#include <core/graphics/utils/light_model.h>
#include <core/graphics/utils/cube_model.h>
#include <core/world/components.h>

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
    ResourceSystem::Init();
    currentScene = Scene(Camera(glm::vec3(0, 0, 10)));
    currentScene.Setup([](Scene &scene)
                       {
                           flecs::entity cube1 = scene.world.entity("cube1");
                           cube1.set<ECSComponents::Transform>({.position = glm::vec3(2.5, 0, 0)});
                           flecs::entity cube2 = scene.world.entity("cube2");
                           cube2.set<ECSComponents::Transform>({.position = glm::vec3(-2.5, 0, 0)});
                           flecs::entity light = scene.world.entity("light");
                           light.set<ECSComponents::Transform>({.position = glm::vec3(0, 10, 0)});

                           cube1.add<ECSComponents::Velocity>();
                           cube2.add<ECSComponents::Velocity>();
                           light.add<ECSComponents::Velocity>();

                           cube1.add<ECSComponents::Renderable>();
                           cube2.add<ECSComponents::Renderable>();
                           light.add<ECSComponents::Renderable>();
                           light.set<ECSComponents::PointLight>({.position = glm::vec3(0, 5, 0),
                                                                 .color = glm::vec3(1),
                                                                 .intensity = 1.0f});

                           cube1.set<ECSComponents::Model>({.model = ResourceSystem::PrepareResource({.path = "hardcoded/cube", .name = "defaults/model/cube_tex", .type = ResourceSystem::RESOURCE_MODEL})});
                           cube2.set<ECSComponents::Model>({.model = ResourceSystem::PrepareResource({.path = "hardcoded/cube", .name = "defaults/model/cube_tex", .type = ResourceSystem::RESOURCE_MODEL})});
                           light.set<ECSComponents::Model>({.model = ResourceSystem::PrepareResource({.path = "hardcoded/cube", .name = "defaults/model/cube", .type = ResourceSystem::RESOURCE_MODEL})});
                           // lambda end
                       });
    currentScene.RegisterSystems();
    startTime = glfwGetTime();
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
      currentScene.Update();

      DrawApplicationPropertiesDebug();
      currentScene.camera.DrawCameraPropertiesDebug();

      renderer.EndFrame();

      window.SwapBuffers();
      // todo dont realy want to poll mouse position, but it will be what it will be

      Input::mouseDeltaX = 0;
      Input::mouseDeltaY = 0;
}
