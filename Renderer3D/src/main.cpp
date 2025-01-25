#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"
#include "Entity.h"
#include "Light.h"
#include "Model.h"
#include "PointLight.h"
#include "Shape.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <memory>

// function declarations
GLFWwindow *initialize_window();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// global constants
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    // create window
    GLFWwindow* window = initialize_window();

    if(window == nullptr)
        return -1;

    // temp: set up our only shader
    Shader baseShader("assets/shaders/BaseVertexShader.vs", "assets/shaders/BaseFragmentShader.fs");

    // set up entities

    shared_ptr<Renderable> crabSpaceshipRenderable = std::make_shared<Model>("assets/models/spaceship_eav_2_crab/scene.gltf");
    auto crabSpaceship = Entity(crabSpaceshipRenderable, glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.05f));

    shared_ptr<Renderable> sphereRenderable = std::make_shared<Shape>(Shape::GetSphere(
            "assets/textures/gold/diffuse.png",
            "assets/textures/gold/specular.png"
    ));
    auto sphere = Entity(sphereRenderable, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(3.5f));

    shared_ptr<Renderable> planeRenderable = std::make_shared<Shape>(Shape::GetPlane(
            "assets/textures/redstone/diffuse.png",
            "assets/textures/redstone/specular.png",
            "assets/textures/redstone/normal.png"
    ));
    auto plane = Entity(planeRenderable, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1000.f));

    // set up lights

    auto light = PointLight(glm::vec3(0.5f, 4.0f, 1.5f), glm::vec3(0.05f));

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // render
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.UpdateShader(baseShader, SCR_WIDTH, SCR_HEIGHT);
        light.UpdateShader(baseShader);

        baseShader.setFloat("material.shininess", 32.0f);

        float radius = 8.0f;
        crabSpaceship.SetPosition(glm::vec3(glm::sin(glfwGetTime() / 2.0f) * radius, 2.0f + 0.3f * glm::cos(glfwGetTime() * 4.0f), glm::cos(glfwGetTime() / 2.0f) * radius));
        crabSpaceship.SetRotation(glm::vec3 (0.0f, glfwGetTime() / 2.0f, 0.0f));

        // camera.SetTarget(glm::vec3(glm::sin(glfwGetTime() / 2.0f) * radius, 2.0, glm::cos(glfwGetTime() / 2.0f) * radius));
        // camera.SetPosition(glm::vec3(0.5f, 3.0f, 1.0f));

        crabSpaceship.Draw(baseShader);
        sphere.Draw(baseShader);
        plane.Draw(baseShader);

        // swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// process window size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// process all input
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// initialize window
GLFWwindow *initialize_window() {
    // initialize and configure GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Renderer3D", NULL, NULL);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    return window;
}