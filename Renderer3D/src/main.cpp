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
#include "DirectionalLight.h"
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

// setup function declarations
void setupShaders();
void setupEntities();
void setupLights();

// scene modification function declarations
void setDay();
void setNight();
void setupCamera();

// enums
enum class CameraMode{
    Controlled,
    Stationary,
    Tracking,
    Following,
};

// global constants
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;
const glm::vec3 POINT_LIGHT_POSITIONS[2] = {glm::vec3(0.5f, 4.0f, 1.5f), glm::vec3(-5.0f, 1.0f, -2.5f)};

// global variables
glm::vec3 skyColor = glm::vec3(0.57f, 0.53f, 0.35f);
CameraMode camMode = CameraMode::Controlled;

// camera
Camera camera(glm::vec3(0.0f, 5.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// shaders
std::unique_ptr<Shader> baseShader, lightSourceShader;

// entities
std::unique_ptr<Entity> crabSpaceship, plane, sphere, lightSpheres[2];

// lights
std::unique_ptr<PointLight> pointLight[2];
std::unique_ptr<DirectionalLight> dirLight;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    // create window
    GLFWwindow* window = initialize_window();

    if(window == nullptr)
        return -1;

    // set up shaders
    setupShaders();

    // set up entities
    setupEntities();

    // set up lights
    setupLights();

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
        glClearColor(skyColor.x, skyColor.y, skyColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setupCamera();

        for(const auto & l : pointLight)
        {
            l->UpdateShader(*baseShader);
        }
        dirLight->UpdateShader(*baseShader);

        baseShader->use();
        baseShader->setFloat("material.shininess", 32.0f);

        float radius = 6.5f;
        crabSpaceship->SetPosition(glm::vec3(glm::sin(glfwGetTime() / 2.0f) * radius, 2.0f + 0.3f * glm::cos(glfwGetTime() * 4.0f), glm::cos(glfwGetTime() / 2.0f) * radius));
        crabSpaceship->SetRotation(glm::vec3 (0.0f, glfwGetTime() / 2.0f, 0.0f));

        crabSpaceship->Draw(*baseShader);
        sphere->Draw(*baseShader);
        plane->Draw(*baseShader);
        for(const auto & lightSphere : lightSpheres)
        {
            lightSphere->Draw(*lightSourceShader);
        }

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

    if(camMode == CameraMode::Controlled)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        setDay();
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        setNight();

    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
        camMode = CameraMode::Controlled;
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        camMode = CameraMode::Stationary;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        camMode = CameraMode::Tracking;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        camMode = CameraMode::Following;
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

    if(camMode == CameraMode::Controlled)
    {
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
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

void setupShaders()
{
    baseShader = std::make_unique<Shader>("assets/shaders/BaseVertexShader.vs", "assets/shaders/BaseFragmentShader.fs");
    lightSourceShader = std::make_unique<Shader>("assets/shaders/BaseVertexShader.vs", "assets/shaders/LightSourceFragmentShader.fs");
}

void setupEntities()
{
    shared_ptr<Renderable> crabSpaceshipRenderable = std::make_shared<Model>("assets/models/spaceship_eav_2_crab/scene.gltf");
    crabSpaceship = std::make_unique<Entity>(crabSpaceshipRenderable, glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.05f));

    shared_ptr<Renderable> sphereRenderable = std::make_shared<Shape>(Shape::GetSphere(
            "assets/textures/gold/diffuse.png",
            "assets/textures/gold/specular.png"
    ));
    sphere = std::make_unique<Entity>(sphereRenderable, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(3.5f));

    shared_ptr<Renderable> planeRenderable = std::make_shared<Shape>(Shape::GetPlane(
            "assets/textures/redstone/diffuse.png",
            "assets/textures/redstone/specular.png",
            "assets/textures/redstone/normal.png"
    ));
    plane = std::make_unique<Entity>(planeRenderable, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1000.f));

    shared_ptr<Renderable> lightSphereRenderable = std::make_shared<Shape>(Shape::GetSphere());
    for(int i = 0; i < 2; i++)
    {
        lightSpheres[i] = std::make_unique<Entity>(lightSphereRenderable, POINT_LIGHT_POSITIONS[i], glm::vec3 (0.0f), glm::vec3(0.2f));
    }
}

void setupLights()
{
    for(int i = 0; i < 2; i++)
    {
        pointLight[i] = std::make_unique<PointLight>(i, POINT_LIGHT_POSITIONS[i]);
    }
    dirLight = std::make_unique<DirectionalLight>(glm::vec3(-0.2f, -1.0f, -0.3f));
}

void setNight()
{
    skyColor = glm::vec3(0.16f, 0.15f, 0.12f);

    dirLight->SetAmbient(glm::vec3(0.0125f));
    dirLight->SetDiffuse(glm::vec3(0.1f));
    dirLight->SetSpecular(glm::vec3(0.125f));
}

void setDay()
{
    skyColor = glm::vec3(0.57f, 0.53f, 0.35f);

    dirLight->SetAmbient(glm::vec3(0.05f));
    dirLight->SetDiffuse(glm::vec3(0.4f));
    dirLight->SetSpecular(glm::vec3(0.5f));
}

void setupCamera(){
    constexpr glm::vec3 StationaryCameraPosition = glm::vec3(-12.0f, 5.0f, 12.0f);
    constexpr glm::vec3 StationaryCameraTarget = glm::vec3(0.0f);

    constexpr glm::vec3 TrackingCameraPosition = glm::vec3(-3.0f, 8.0f, 3.0f);

    constexpr glm::vec4 BaseVector = glm::vec4(glm::vec3(0.0f), 1.0f);
    constexpr glm::vec3 Shift = glm::vec3(-100.0f, 40.0f, 0.0f);

    switch (camMode) {
        case CameraMode::Stationary:
            camera.SetPosition(StationaryCameraPosition);
            camera.SetTarget(StationaryCameraTarget);
            break;
        case CameraMode::Tracking:
            camera.SetPosition(TrackingCameraPosition);
            camera.SetTarget(crabSpaceship->GetPosition());
            break;
        case CameraMode::Following:
            glm::vec3 position = glm::translate(crabSpaceship->GenerateModelMatrix(), Shift) * BaseVector;

            camera.SetPosition(position);
            camera.SetTarget(crabSpaceship->GetPosition());
            break;
    }

    camera.UpdateShader(*baseShader, SCR_WIDTH, SCR_HEIGHT);
    camera.UpdateShader(*lightSourceShader, SCR_WIDTH, SCR_HEIGHT);
}