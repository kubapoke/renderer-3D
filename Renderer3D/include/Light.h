#ifndef RENDERER3D_LIGHT_H
#define RENDERER3D_LIGHT_H

#include "Shader.h"

class Light{
public:
    Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : ambient(ambient), diffuse(diffuse), specular(specular) {}

    virtual void UpdateShader(Shader& shader) = 0;

    void SetAmbient(glm::vec3 amb) { ambient = amb; }
    void SetDiffuse(glm::vec3 dif) { diffuse = dif; }
    void SetSpecular(glm::vec3 spe) { specular = spe; }
protected:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

#endif //RENDERER3D_LIGHT_H
