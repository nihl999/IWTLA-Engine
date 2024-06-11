#define GLFW_INCLUDE_NONE
#include "Application.h"
#include <stdio.h>
#include <GLAD/glad.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include "Input.h"
#include "Scene.h"
#include "utils/File/File.h"
#include "Shader.h"

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
    ImGui::Begin("Entities Properties");
    ImGui::Text("Entities Count %d", currentScene.entities.size());
    if (ImGui::TreeNode("Entity List: "))
    {
        for (int n = 0; n < currentScene.entities.size(); n++)
        {
            ImGui::PushID(&currentScene.entities[n]);
            if (ImGui::TreeNode(&currentScene.entities[n], "Entity %d", n))
            {
                ImGui::Text("Position");
                ImGui::DragFloat3("", &currentScene.entities[n].position[0], 0.5f, 0.0f, 0.0f, "%.2f");
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        ImGui::TreePop();
    }

    ImGui::End();
}

Application::Application() : currentScene(Scene())
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
    // // todo horrible
    // for (CubeModel model : models)
    // {
    //     for (float vert : model.mesh.vertices)
    //     {
    //         vertices.push_back(vert);
    //     }
    // }
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void *)0);

    char *vertexShader = FileUtils::readFile("vertex.shader");
    char *fragmentShader = FileUtils::readFile("fragment.shader");

    GLuint vertexShaderId = Shader::createShader(vertexShader, GL_VERTEX_SHADER);
    free(vertexShader);
    GLuint fragmentShaderId = Shader::createShader(fragmentShader, GL_FRAGMENT_SHADER);
    free(fragmentShader);

    GLuint programId = Shader::createProgram(vertexShaderId, fragmentShaderId, true);
    glUseProgram(programId);

    //--
    glBufferData(GL_ARRAY_BUFFER, sizeof(currentScene.entities[0].mesh.vertices), &currentScene.entities[0].mesh.vertices[0], GL_STATIC_DRAW);
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
    glm::mat4 viewProj = currentScene.camera.getViewMatrix();

    glUniformMatrix4fv(2,
                       1,
                       false,
                       &viewProj[0][0]);
    // todo horrible

    glClearColor(0, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (CubeModel model : currentScene.entities)
    {
        glUniformMatrix4fv(3,
                           1,
                           GL_FALSE,
                           &model.GetModelMatrix()[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, (sizeof(model.mesh.vertices) * 3));
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