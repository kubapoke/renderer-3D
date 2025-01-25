#ifndef RENDERER3D_ENTITY_H
#define RENDERER3D_ENTITY_H

#include "Renderable.h"
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Entity {
public:
    explicit Entity(std::shared_ptr<Renderable> renderable,
                    glm::vec3 position = glm::vec3(0.0f),
                    glm::vec3 rotation = glm::vec3(0.0f),
                    glm::vec3 scale = glm::vec3(1.0f))
            : renderable(std::move(renderable)), position(position), rotation(rotation), scale(scale) {}

    virtual ~Entity() = default;

    void Draw(Shader& shader) {
        shader.use();

        shader.setMat4("model", GenerateModelMatrix());
        renderable->Draw(shader);
    }

    // Setters for transformations
    void SetPosition(const glm::vec3& pos) { position = pos; }
    void SetRotation(const glm::vec3& rot) { rotation = rot; }
    void SetScale(const glm::vec3& scl) { scale = scl; }

    glm::vec3 GetPosition() const {
        return position;
    }

    glm::mat4 GenerateModelMatrix() const {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, rotation.x, glm::vec3(1.0, 0.0, 0.0));
        model = glm::rotate(model, rotation.y, glm::vec3(0.0, 1.0, 0.0));
        model = glm::rotate(model, rotation.z, glm::vec3(0.0, 0.0, 1.0));
        model = glm::scale(model, scale);
        return model;
    }

private:
    std::shared_ptr<Renderable> renderable;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

#endif // RENDERER3D_ENTITY_H
