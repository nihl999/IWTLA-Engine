#define GLFW_INCLUDE_NONE
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils/File/File.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include "Camera.h"
#include "TempModel.h"
#include <vector>

// todo all should be on application
GLFWwindow *window;
int windowWidth = 1280;
int windowHeight = 720;
float startTime = 0;
float lastFrameTime = 0;
float deltaTime = 0;

Camera cam = Camera();

std::vector<CubeModel> models = {CubeModel(), CubeModel(glm::vec3(5, 0, -5))};

// todo this should be on entity manager

void DrawEntitiesProperties()
{
    ImGui::Begin("Entities Properties");
    ImGui::Text("Entities Count %d", models.size());
    if (ImGui::TreeNode("Entity List: "))
    {
        for (int n = 0; n < models.size(); n++)
        {
            ImGui::PushID(&models[n]);
            if (ImGui::TreeNode(&models[n], "Entity %d", n))
            {
                ImGui::Text("Position");
                ImGui::DragFloat3("", &models[n].position[0], 0.5f, 0.0f, 0.0f, "%.2f");
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        ImGui::TreePop();
    }

    ImGui::End();
};

struct InputAxis
{
    int axisId = 0;
    int minusKey = 0;
    bool minusKeyPressed = false;
    int plusKey = 0;
    bool plusKeyPressed = false;
    int currentAxisValue = 0;
} typedef InputAxis;

const int axisArrayLength = 5;
InputAxis axis[axisArrayLength] = {
    {
        .axisId = 1,
        .minusKey = GLFW_KEY_S,
        .plusKey = GLFW_KEY_W,
    },
    {
        .axisId = 2,
        .minusKey = GLFW_KEY_A,
        .plusKey = GLFW_KEY_D,
    },
    {
        .axisId = 3,
        .minusKey = GLFW_KEY_LEFT_SHIFT,
        .plusKey = GLFW_KEY_SPACE,
    },
};

// todo this should be on input and application
bool capMouse = false;
bool enteringCap = true;
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_J && action == GLFW_PRESS)
    {
        if (!capMouse)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            capMouse = !capMouse;
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            capMouse = !capMouse;
            enteringCap = true;
        }
    }
    for (int i = 0; i < axisArrayLength; i++)
    {
        if (key == axis[i].minusKey && action == GLFW_PRESS)
        {
            axis[i].minusKeyPressed = true;
            axis[i].currentAxisValue -= 1;
        }
        if (key == axis[i].plusKey && action == GLFW_PRESS)
        {
            axis[i].plusKeyPressed = true;
            axis[i].currentAxisValue += 1;
        }
        if (key == axis[i].minusKey && action == GLFW_RELEASE)
        {
            axis[i].minusKeyPressed = false;
            axis[i].currentAxisValue += 1;
        }
        if (key == axis[i].plusKey && action == GLFW_RELEASE)
        {
            axis[i].plusKeyPressed = false;
            axis[i].currentAxisValue -= 1;
        }
    }
}
float lastX = windowWidth / 2;
float lastY = windowHeight / 2;
float sensitivity = 0.1f;
float xoffset = 0;
float yoffset = 0;
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (!capMouse)
        return;
    if (enteringCap)
    {
        lastX = xpos;
        lastY = ypos;
        enteringCap = false;
    }
    xoffset = xpos - lastX;
    yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;
}

void window_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void DrawApplicationPropertiesDebug()
{
    ImGui::Begin("Application Properties");
    ImGui::Checkbox("Capture Mouse", &capMouse);
    // todo Should be on some inputs tab maybe? or application handles... anyway
    ImGui::SliderFloat("Mouse Sensitivity", &sensitivity, 0.01f, 3.0f, "%.2f");
    ImGui::Text("Mouse X: %.2f", lastX);
    ImGui::Text("Mouse Y: %.2f", lastY);
    ImGui::Text("Mouse X Offset: %.3f", xoffset);
    ImGui::Text("Mouse Y Offset: %.3f", yoffset);
    ImGui::End();
}

void update();

int main(void)
{

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwSetWindowAspectRatio(window, 16, 9);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    startTime = glfwGetTime();

    gladLoadGL();

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

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

    glm::mat4 viewProj = cam.getViewMatrix();

    glUniformMatrix4fv(2,
                       1,
                       false,
                       &viewProj[0][0]);
    //--
    glBufferData(GL_ARRAY_BUFFER, sizeof(models[0].mesh.vertices), &models[0].mesh.vertices[0], GL_STATIC_DRAW);

    while (!glfwWindowShouldClose(window))
    {
        update();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

void update()
{
    /* Poll for and process events */
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    deltaTime = glfwGetTime() - lastFrameTime;
    lastFrameTime = glfwGetTime();

    cam.move(glm::vec3(axis[1].currentAxisValue * deltaTime, axis[2].currentAxisValue * deltaTime, axis[0].currentAxisValue * deltaTime));
    cam.updateYawPitch(xoffset, yoffset);
    xoffset = 0;
    yoffset = 0;
    cam.update();
    glm::mat4 viewProj = cam.getViewMatrix();

    glUniformMatrix4fv(2,
                       1,
                       false,
                       &viewProj[0][0]);
    // todo horrible

    glClearColor(0, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (CubeModel model : models)
    {
        glUniformMatrix4fv(3,
                           1,
                           GL_FALSE,
                           &model.GetModelMatrix()[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, (sizeof(models[0].mesh.vertices) * 3));
    }
    /* Render here */

    DrawApplicationPropertiesDebug();
    DrawEntitiesProperties();
    cam.DrawCameraPropertiesDebug();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
}
