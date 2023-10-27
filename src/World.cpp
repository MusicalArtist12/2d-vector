#include "World.h"
#include "Render.h"

#include "Window.h"
#include "Shader.h"

extern render Render;
extern window Window;
extern world World;


glm::mat4 physObject::modelMatrix() {
    glm::mat4 model = transformMatrix;

    model = glm::translate(glm::mat4(1.0f), pos) * model;

    return model;
}

glm::vec3 physObject::forceSum() {
    

    if(isStatic) return glm::vec3(0.0f);

    glm::vec3 forces = glm::vec3(0.0f);

    for(int i = 0; i < forceVectors.size(); i++) {
        forces += forceVectors.getRef(i);
    }

    return forces;
}

void physObject::resolveCollision(physObject& obj) {
    // assuming they're colliding, current implementation is perfectly kinetic
        // Ki = Kf
    glm::vec3& forceA = forceVectors.add(obj.getID(), glm::vec3(0.0f));
    glm::vec3& forceB = obj.forceVectors.add(getID(), glm::vec3(0.0f));


    if(!isColliding(obj)) {
        forceA = glm::vec3(0.0f);
        forceB = glm::vec3(0.0f);
        return;
    }

    if(glm::sign(vel) == glm::sign(obj.pos - pos)) {
        forceA = (-1.0f * (forceSum() - forceA)) - (mass * vel)/Window.deltaTime;
        
        forceB = (-1.0f * (forceSum()));
    }
}

// ******************************************************************

physObject world::pullItem(std::string id) {
    return objectTable.remove(id);
}

void world::update() {

    for(int i = 0; i < objectTable.size(); i++) {
        physObject& iObject = objectTable.getRef(i);
        calculateForces(iObject);
    }

    for(int i = 0; i < objectTable.size(); i++) {
        physObject& iObject = objectTable.getRef(i);
        iObject.id = objectTable.getID(i);

        if(usePhysics && !iObject.isStatic) calculateMovement(iObject);        
        Render.drawMesh(iObject.myMesh, iObject.modelMatrix());
    }
}

void world::calculateMovement(physObject& obj) {
    glm::vec3 accel = obj.forceSum() / obj.mass;

    obj.vel += accel * Window.deltaTime;
    obj.pos += obj.vel * Window.deltaTime;
}

void world::calculateForces(physObject& obj) {
    
    // generate default forces if they don't exist
    //glm::vec3& normalFloor = obj.forceVectors.add("physEngine normalFloor", glm::vec3(0.0f));
    glm::vec3& gravForce = obj.forceVectors.add("physEngine gravForce", glm::vec3(0.0f));

    gravForce = glm::vec3(0.0f, grav, 0.0f) * obj.mass;

    for(int i = 0; i < objectTable.size(); i++) {
        physObject& iObject = objectTable.getRef(i);

        if(iObject.getID() != obj.getID()) {
            obj.resolveCollision(iObject);
        }
    }

}

float world::distance(glm::vec3 ptA, glm::vec3 ptB) {
    glm::vec3 delta = glm::vec3(ptA.x - ptB.x, ptA.y - ptB.y, ptA.z - ptB.z);

    return (float)sqrt(pow(delta.x,2) + pow(delta.y,2));
}

bool physObject::isColliding(physObject& obj) {
    if(distance(pos, obj.pos) > getRadius() + obj.getRadius()) return false;

    return true;
}


