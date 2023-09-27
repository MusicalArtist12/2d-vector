#include "physicsEngine/PhysObject.h"

glm::mat4 physObject::modelMatrix() {
    glm::mat4 model(1.0f);

    model = glm::translate(model, pos);

    return model;
}