#ifndef RENDERER3D_POSITIONALCAMERA_H
#define RENDERER3D_POSITIONALCAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class PositionalCamera
{
public:
    glm::vec3 Eye;
    glm::vec3 Center;
    glm::vec3 Up;

    PositionalCamera(glm::vec3 eye = glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
    {
        Eye = eye;
        Center = center;
        Up = up;
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Eye, Center, Up);
    }

    void SetEye(glm::vec3 eye)
    {
        Eye = eye;
    }

    void SetCenter(glm::vec3 center)
    {
        Center = center;
    }
};

#endif //RENDERER3D_POSITIONALCAMERA_H
