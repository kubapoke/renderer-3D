#ifndef RENDERER3D_DIRECTIONALLIGHT_H
#define RENDERER3D_DIRECTIONALLIGHT_H

#include "Light.h"

class DirectionalLight : public Light{
public:
    explicit DirectionalLight(glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3 ambient = glm::vec3(0.05f),
                              glm::vec3 diffuse = glm::vec3(0.4f), glm::vec3 specular = glm::vec3(0.5f))
                              : direction(direction), Light(ambient, diffuse, specular) {}

    void UpdateShader(Shader& shader) override
    {
        shader.use();

        const std::string name = "dirLight.";

        shader.setVec3(name + "direction", direction);
        shader.setVec3(name + "ambient", ambient);
        shader.setVec3(name + "diffuse", diffuse);
        shader.setVec3(name + "specular", specular);
    }

    void SetDirection(glm::vec3 dir) { direction = dir; }
private:
    glm::vec3 direction;
};

#endif //RENDERER3D_DIRECTIONALLIGHT_H
