#include "physObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "utils/Dictionary_impl.h"

bool physObject::xBounds(float x_min, float x_max) {
    if( (x_min + myMesh->radius) < pos.x && pos.x < (x_max - myMesh->radius) ){
        return true;
    }

    return false;
}

bool physObject::yBounds(float y_min, float y_max) {
    //std::cout << y_min + myMesh->radius << " < " << pos.y << " < " << y_max + myMesh->radius << std::endl;

    if( (y_min + myMesh->radius) < pos.y && pos.y < (y_max - myMesh->radius) ){
        return true;
    }

    return false;
}

int physObject::xAxisRelation(float axis) {
    if( (axis < pos.x - myMesh->radius) ) return 1;
    if( (axis > pos.x + myMesh->radius) ) return -1;
    return 0;
}

int physObject::yAxisRelation(float axis) {
    if( (axis < pos.y - myMesh->radius) ) return 1;
    if( (axis > pos.y + myMesh->radius) ) return -1;
    return 0;
}

glm::mat4 physObject::modelMatrix() {
    glm::mat4 model(1.0f);

    model = glm::translate(model, pos);

    return model;
}

