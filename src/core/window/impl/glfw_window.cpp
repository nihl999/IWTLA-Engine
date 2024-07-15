#include "imgui.h"
#define GLFW_INCLUDE_NONE
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <Input.h>
#include <core/window/window.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <stdio.h>
#include <stdlib.h>

// todo i dont like this file.

GLFWwindow *window;

void WindowSizeCallback(GLFWwindow *window, int width, int height)
{
  ImGuiIO &io = ImGui::GetIO();
  io.DisplaySize = ImVec2((float)width, (float)height);
  glViewport(0, 0, width, height);
}

Window::Window(int _width, int _height) : width(_width), height(_height), aspectRatio((float)_width / (float)height)
{
    if (!glfwInit())
    {
        printf("Error initializing GLFW library!");
        exit(1);
    }

    window = glfwCreateWindow(width, height, "Nurget", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        printf("Error initializing window with GLFW!");
        exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwSetWindowAspectRatio(window, width, height);
    glfwSetKeyCallback(window, &Input::KeyCallback);
    glfwSetCursorPosCallback(window, &Input::MouseCallback);
    glfwSetWindowSizeCallback(window, WindowSizeCallback);

    gladLoadGL();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
};

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::PoolEvents()
{
    glfwPollEvents();
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(window);
}

void Window::NewFrame()
{
    ImGui_ImplGlfw_NewFrame();
}

Window::~Window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
};