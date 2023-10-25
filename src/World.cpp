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

// ******************************************************************


physObject world::pullItem(std::string id) {
    return objectTable.remove(id);
}

void world::update() {

    for(int i = 0; i < objectTable.size(); i++) {
        physObject& iObject = objectTable.getRef(i);
        iObject.id = objectTable.getID(i);

        if(usePhysics) calculateMovement(iObject);        
        Render.drawMesh(iObject.myMesh, iObject.modelMatrix());
    }

    for(int i = 0; i < objectTable.size(); i++) {
        physObject& iObject = objectTable.getRef(i);
        calculateForces(iObject);
    }
}

void world::calculateMovement(physObject& obj) {
    obj.accel = obj.forceSum() / obj.mass;

    obj.vel += obj.accel * Window.deltaTime;
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
            resolveCollision(obj, iObject);
        }
    }


    obj.accel = glm::vec3(0.0f);


    
}

float world::distance(glm::vec3 ptA, glm::vec3 ptB) {
    glm::vec3 delta = glm::vec3(ptA.x - ptB.x, ptA.y - ptB.y, ptA.z - ptB.z);

    return (float)sqrt(pow(delta.x,2) + pow(delta.y,2));
}

bool world::isColliding(physObject& objA, physObject& objB) {
    if(distance(objA.pos, objB.pos) > objA.getRadius() + objB.getRadius()) return false;

    return true;
}

void world::resolveCollision(physObject& objA, physObject& objB) {
    // assuming they're colliding, current implementation is perfectly kinetic
        // Ki = Kf
    glm::vec3& forceA = objA.forceVectors.add(objB.getID(), glm::vec3(0.0f)) = glm::vec3(0.0f);
    glm::vec3& forceB = objB.forceVectors.add(objA.getID(), glm::vec3(0.0f)) = glm::vec3(0.0f);

    if(!isColliding(objA, objB)) {
        return;
    }
    
    glm::vec3 iVelA = objA.vel;
    glm::vec3 iVelB = objB.vel;

    glm::vec3 dVA =  ((objA.mass - objB.mass)*iVelA + (2 * objB.mass * iVelB))/(objA.mass + objB.mass);
    glm::vec3 dVB =  ((objB.mass - objA.mass)*iVelB + (2 * objA.mass * iVelA))/(objA.mass + objB.mass);

    // this is an impulse, so its going to be a force = m dv/dt

    forceA = objA.mass * dVA/Window.deltaTime - objB.forceSum();
    forceB = objB.mass * dVB/Window.deltaTime - objA.forceSum();


}
