#include "PhysicsEngine.h"

#include "glCore/WindowEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace physics {
    float grav = 0.0f;
    float ground = 0.0f;
    float ceiling = 1000.0f;
}

void physics::calculatePhysics(physObject& obj) {
    //float* accel[] = { &obj.accel.x, &obj.accel.y };
    
    float tmp_accel = obj.accel.y;

    if(obj.inYBounds(ground, ceiling)) {
        obj.accel.y += grav;
        std::cout << "using gravity" << std::endl;
    } else {
        if(obj.vel.y < 0.0) obj.vel.y = 0.0;
    }

    obj.vel += obj.accel * window::deltaTime;


    obj.pos += (obj.vel * window::deltaTime);

    obj.accel.y = tmp_accel;
}

bool physObject::inXBounds(float x_min, float x_max) {
    std::cout << x_min + myMesh->radius << " < " << pos.x << " < " << x_max + myMesh->radius << std::endl;

    if( (x_min + myMesh->radius) < pos.x && pos.x < (x_max - myMesh->radius) ){
        return true;
    }

    return false;
}

bool physObject::inYBounds(float y_min, float y_max) {
    std::cout << y_min + myMesh->radius << " < " << pos.y << " < " << y_max + myMesh->radius << std::endl;

    if( (y_min + myMesh->radius) < pos.y && pos.y < (y_max - myMesh->radius) ){
        return true;
    }

    return false;
}

glm::mat4 physObject::modelMatrix() {
    glm::mat4 model(1.0f);

    model = glm::translate(model, pos);

    return model;
}

