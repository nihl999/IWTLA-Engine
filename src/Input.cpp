#include "Input.h"

bool Input::capturingMouse = false;
bool Input::firstEntry = true;
float Input::sensitivity = 0.1f;
float Input::mousePositionX = 0;
float Input::mousePositionY = 0;
float Input::lastMousePositionX = 0;
float Input::lastMousePositionY = 0;
float Input::mouseDeltaX = 0;
float Input::mouseDeltaY = 0;
;

void Input::MouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (!capturingMouse)
        return;
    if (firstEntry)
    {
        mousePositionX = lastMousePositionX = xpos;
        mousePositionY = lastMousePositionY = ypos;
        firstEntry = false;
    }
    mouseDeltaX = xpos - lastMousePositionX;
    mouseDeltaY = lastMousePositionY - ypos;
    lastMousePositionX = xpos;
    lastMousePositionY = ypos;

    mouseDeltaX *= sensitivity;
    mouseDeltaY *= sensitivity;
}

// todo redo this as event system, for now will have glfw specific things here, im cansado
void Input::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_J && action == GLFW_PRESS)
    {
        if (!capturingMouse)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            capturingMouse = !capturingMouse;
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            capturingMouse = !capturingMouse;
            firstEntry = true;
        }
    }
    for (auto i = axis.begin(); i != axis.end(); i++)
    {
        if (key == i->second.minusKey && action == GLFW_PRESS)
        {
            i->second.minusKeyPressed = true;
            i->second.currentAxisValue -= 1;
        }
        if (key == i->second.plusKey && action == GLFW_PRESS)
        {
            i->second.plusKeyPressed = true;
            i->second.currentAxisValue += 1;
        }
        if (key == i->second.minusKey && action == GLFW_RELEASE)
        {
            i->second.minusKeyPressed = false;
            i->second.currentAxisValue += 1;
        }
        if (key == i->second.plusKey && action == GLFW_RELEASE)
        {
            i->second.plusKeyPressed = false;
            i->second.currentAxisValue -= 1;
        }
    }
}

std::unordered_map<std::string, InputAxis> Input::axis = {
    {"Vertical",
     {
         .minusKey = GLFW_KEY_S,
         .plusKey = GLFW_KEY_W,
     }},
    {"Horizontal",
     {
         .minusKey = GLFW_KEY_A,
         .plusKey = GLFW_KEY_D,
     }},
    {"Fly",
     {
         .minusKey = GLFW_KEY_LEFT_SHIFT,
         .plusKey = GLFW_KEY_SPACE,
     }},
};