#include "physicsEngine/PhysObject.h"

#include <iostream>

bool physObject::inBounds(float x, float y) {
    x = x - pos.x;
    y = y - pos.y;

    // check radius
    float distance = sqrt( pow(x, 2) + pow(y, 2) );

    // if in radius, continue. otherwise return false
    if( distance > this->myMesh->radius) return false;

    // Two cases:
        // direction from the center of the 


    return true;
}

glm::mat4 physObject::modelMatrix() {
    glm::mat4 model(1.0f);

    model = glm::translate(model, pos);

    return model;
}