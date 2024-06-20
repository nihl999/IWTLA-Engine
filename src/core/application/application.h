#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <core/world/scene.h>
#include <core/window/window.h>
#include <core/graphics/renderer/renderer.h>
#include <core/resources/resource_manager.h>

class Application
{
private:
    Window window;
    Renderer renderer;
    ResourceManager *resourceManager;
    float lastFrameTime = 0;
    float startTime = 0;

public:
    bool shouldRun = true;
    float deltaTime = 0;
    Scene currentScene;
    Application();

private:
    void DrawApplicationPropertiesDebug();

public:
    void Update();
};
