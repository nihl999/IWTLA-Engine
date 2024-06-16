#ifndef APPL_CLASS
#define APPL_CLASS

// todo untie app - renderer - window
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <core/world/scene.h>

class Application
{
private:
    // todo should go to a window only class probably - maybe stop oop and do struct c like
    GLFWwindow *window;
    int windowWidth = 1280;
    int widthAspectRatio = 16;
    int windowHeight = 720;
    int heightAspectRatio = 9;
    float lastFrameTime = 0;
    float startTime = 0;

public:
    bool shouldRun = false;
    float deltaTime = 0;
    Scene currentScene;
    Application();

private:
    void DrawApplicationPropertiesDebug();
    static void WindowSizeCallback(GLFWwindow *window, int width, int height);

public:
    void Update();
    void CleanUp();
};

#endif