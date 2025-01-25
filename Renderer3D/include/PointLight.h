#ifndef RENDERER3D_POINTLIGHT_H
#define RENDERER3D_POINTLIGHT_H

#include "Light.h"

class PointLight : public Light{
public:
    explicit PointLight(glm::vec3 position = glm::vec3(0.0f), glm::vec3 ambient = glm::vec3(0.0f),
               glm::vec3 diffuse = glm::vec3(0.8f), glm::vec3 specular = glm::vec3(1.0f),
               float constant = 1.0f, float linear = 0.09f,
               float quadratic = 0.032f)
    : position(position), Light(ambient, diffuse, specular), constant(constant), linear(linear), quadratic(quadratic) {}

    void UpdateShader(Shader& shader) override
    {
        shader.use();

        const std::string name = "pointLight.";

        shader.setVec3(name + "position", position);
        shader.setVec3(name + "ambient", ambient);
        shader.setVec3(name + "diffuse", diffuse);
        shader.setVec3(name + "specular", specular);
        shader.setFloat(name + "constant", constant);
        shader.setFloat(name + "linear", linear);
        shader.setFloat(name + "quadratic", quadratic);
    }

    void SetPosition(glm::vec3 pos) { position = pos; }
    void SetConstant(float con) { constant = con; }
    void SetLinear(float lin) { linear = lin; }
    void SetQuadratic(float qua) { quadratic = qua; }
private:
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;
};

#endif //RENDERER3D_POINTLIGHT_H
