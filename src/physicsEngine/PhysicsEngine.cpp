#include "PhysicsEngine.h"

#include "glCore/WindowEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "utils/Dictionary_impl.h"

namespace physics {
    float grav = 0.0f;
    float ground = 0.0f;
    float ceiling = 1000.0f;
}

void physics::calculateForces(physObject& obj) {
    
    // force calculations
    std::vector<glm::vec3> forces;

    // generate default forces if they don't exist
    glm::vec3& normalFloor = obj.forceVectors.add("physEngine normalFloor", glm::vec3(0.0f));
    glm::vec3& gravForce = obj.forceVectors.add("physEngine gravForce", glm::vec3(0.0f));

    gravForce = glm::vec3(0.0f, grav, 0.0f) * obj.mass;

    if(obj.yAxisRelation(ground) <= 0) {
        if(gravForce.y <= 0) 
            normalFloor = gravForce * -1.0f;
        
        if(obj.vel.y < 0.0f) obj.vel.y = 0.0f;

    } else {
        normalFloor = glm::vec3(0.0f);
    }

    obj.accel = glm::vec3(0.0f);

    std::vector<std::string> forceNames = obj.forceVectors.nameList();
    
    obj.forceSum = glm::vec3(0.0f);

    for(int i = 0; i < forceNames.size(); i++) {
        obj.forceSum += obj.forceVectors.entry(forceNames[i]);
    }

}

void physics::calculateMovement(physObject& obj) {
    obj.accel = obj.forceSum / obj.mass;

    obj.vel += obj.accel * window::deltaTime;
    obj.pos += obj.vel * window::deltaTime;
}

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

