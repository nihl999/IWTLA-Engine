#pragma once
#include <core/defines.h>
#include <glfw/glfw3.h>
#include <string>
#include <unordered_map>

struct OUROAPI InputAxis {
  int axisId = 0;
  int minusKey = 0;
  bool minusKeyPressed = false;
  int plusKey = 0;
  bool plusKeyPressed = false;
  int currentAxisValue = 0;
};

class OUROAPI Input {
public:
    static bool capturingMouse;
    static bool firstEntry;
    static float mousePositionX;
    static float mousePositionY;
    static float mouseDeltaX;
    static float mouseDeltaY;
    static float sensitivity;
    static std::unordered_map<std::string, InputAxis> axis;

private:
    static float lastMousePositionX;
    static float lastMousePositionY;

private:
public:
    static void MouseCallback(GLFWwindow *window, double xpos, double ypos);
    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
};