#ifndef RENDERER3D_SPOTLIGHT_H
#define RENDERER3D_SPOTLIGHT_H

#include "Light.h"

class SpotLight : public Light{
public:
    explicit SpotLight(glm::vec3 position = glm::vec3(0.0f),
                       glm::vec3 direction = glm::vec3(-1.0f, 0.0f, 0.0f),
                       float cutOff = glm::cos(glm::radians(12.5f)),
                       float outerCutOff = glm::cos(glm::radians(15.0f)),
                       glm::vec3 ambient = glm::vec3(0.0f),
                       glm::vec3 diffuse = glm::vec3(0.8f),
                       glm::vec3 specular = glm::vec3(1.0f),
                       float constant = 1.0f,
                       float linear = 0.09f,
                       float quadratic = 0.032f)
    : position(position), direction(direction), cutOff(cutOff), outerCutOff(outerCutOff),
    Light(ambient, diffuse, specular), constant(constant), linear(linear), quadratic(quadratic) {}

    explicit SpotLight(int id,
                       glm::vec3 position = glm::vec3(0.0f),
                       glm::vec3 direction = glm::vec3(-1.0f, 0.0f, 0.0f),
                       float cutOff = glm::cos(glm::radians(12.5f)),
                       float outerCutOff = glm::cos(glm::radians(15.0f)),
                       glm::vec3 ambient = glm::vec3(0.0f),
                       glm::vec3 diffuse = glm::vec3(0.8f),
                       glm::vec3 specular = glm::vec3(1.0f),
                       float constant = 1.0f,
                       float linear = 0.09f,
                       float quadratic = 0.032f)
            : position(position), direction(direction), cutOff(cutOff), outerCutOff(outerCutOff),
              Light(id, ambient, diffuse, specular), constant(constant), linear(linear), quadratic(quadratic) {}

    void UpdateShader(Shader& shader) override
    {
        shader.use();

        if(id < 0)
        {
            const std::string name = "spotLight";

            shader.setVec3(name + ".position", position);
            shader.setVec3(name + ".direction", direction);
            shader.setVec3(name + ".ambient", ambient);
            shader.setVec3(name + ".diffuse", diffuse);
            shader.setVec3(name + ".specular", specular);
            shader.setFloat(name + ".cutOff", cutOff);
            shader.setFloat(name + ".outerCutOff", outerCutOff);
            shader.setFloat(name + ".constant", constant);
            shader.setFloat(name + ".linear", linear);
            shader.setFloat(name + ".quadratic", quadratic);
        }
        else
        {
            const std::string name = "spotLights";
            const std::string id_str = std::to_string(id);

            shader.setVec3(name + "[" + id_str + "].position", position);
            shader.setVec3(name + "[" + id_str + "].direction", direction);
            shader.setVec3(name + "[" + id_str + "].ambient", ambient);
            shader.setVec3(name + "[" + id_str + "].diffuse", diffuse);
            shader.setVec3(name + "[" + id_str + "].specular", specular);
            shader.setFloat(name + "[" + id_str + "].cutOff", cutOff);
            shader.setFloat(name + "[" + id_str + "].outerCutOff", outerCutOff);
            shader.setFloat(name + "[" + id_str + "].constant", constant);
            shader.setFloat(name + "[" + id_str + "].linear", linear);
            shader.setFloat(name + "[" + id_str + "].quadratic", quadratic);
        }
    }

    void SetPosition(glm::vec3 pos) { position = pos; }
    void SetDirection(glm::vec3 dir) { direction = dir; }
    void SetCutOff(float cut) { cutOff = cut; }
    void SetOuterCutOff(float cut) { outerCutOff = cut; }
    void SetConstant(float con) { constant = con; }
    void SetLinear(float lin) { linear = lin; }
    void SetQuadratic(float qua) { quadratic = qua; }
private:
    glm::vec3 position;
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
};

#endif //RENDERER3D_SPOTLIGHT_H
