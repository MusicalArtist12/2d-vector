#include "World.h"
#include "Render.h"

#include "Shader.h"

#include <set>

extern render Render;
extern world World;


glm::mat4 physObject::modelMatrix() {
    glm::mat4 model = transformMatrix;

    model = glm::translate(glm::mat4(1.0f), pos) * model;

    return model;
}

glm::vec3 physObject::forceSum() {
    glm::vec3 forces = glm::vec3(0.0f);

    if(isStatic) return forces;

    for(int i = 0; i < forceVectors.size(); i++) {
        if(!World.isValidForce(forceVectors.getID(i))) forceVectors.remove(forceVectors.getID(i)); 
        else forces += forceVectors.getRef(i);
    }


    return forces;
}

// ******************************************************************

void world::update(float deltaTime) {
    for(int i = 0; i < countsPerFrame; i++) {
        updateMovement(deltaTime/countsPerFrame);
    }

    draw();
}


void world::updateMovement(float deltaTime) {
    for(int i = 0; i < objectTable.size(); i++) {
        physObject& iObject = objectTable.getRef(i);
        addGravity(iObject);

        for(int j = 0; j < objectTable.size(); j++) {
            physObject& jObject = objectTable.getRef(j);

            if(isColliding(iObject, jObject)) {
                if( !iObject.forceVectors.hasEntry(jObject.getID()) || 
                    !jObject.forceVectors.hasEntry(iObject.getID()) ){
                        resolveCollision(iObject, jObject, deltaTime);
                        
                    }

                
            } 
            
            else {
                if( iObject.forceVectors.hasEntry(jObject.getID()) ) iObject.forceVectors.remove(jObject.getID());
                if( jObject.forceVectors.hasEntry(iObject.getID()) ) jObject.forceVectors.remove(iObject.getID());
            }

        }

        
    }

    for(int i = 0; i < objectTable.size(); i++) {
        physObject& iObject = objectTable.getRef(i);

        if(usePhysics && !iObject.isStatic) calculateMovement(iObject, deltaTime);  
    }
}

void world::draw() {
    for(int i = 0; i < objectTable.size(); i++) {
        physObject& iObject = objectTable.getRef(i);

        Render.drawMesh(iObject.myMesh, iObject.modelMatrix());
    }
}

void world::resolveCollision(physObject& objA, physObject& objB, float deltaTime) {
    // assuming they're colliding, current implementation is perfectly kinetic
        // Ki = Kf
    glm::vec3& forceA = objA.forceVectors.add( objB.getID(), glm::vec3(0.0f) );
    glm::vec3& forceB = objB.forceVectors.add( objA.getID(), glm::vec3(0.0f) );

    float angle = glm::abs(glm::atan((objA.pos.y-objB.pos.y)/(objA.pos.x-objB.pos.x)));

    float momentumAngleA;
    float momentumAngleB;

    if(objA.momentum().x == 0) {
        momentumAngleA = glm::atan(objA.momentum().y/0.0001);
    } else {
        momentumAngleA = glm::atan(objA.momentum().y/objA.momentum().x);
    }

    if(objB.momentum().y == 0) {
        momentumAngleB = glm::atan(objB.momentum().y/0.0001);
    } else {
        momentumAngleB = glm::atan(objB.momentum().y/objB.momentum().x);
    }

    // add other's momentum
    forceA = glm::sin(angle) * glm::length(objB.momentum()) * glm::vec3(glm::cos(momentumAngleB - angle), glm::sin(momentumAngleB - angle), 0.0f); 
    forceB = glm::sin(angle) * glm::length(objA.momentum()) * glm::vec3(glm::cos(momentumAngleA - angle), glm::sin(momentumAngleA - angle), 0.0f); 

    // remove own momentum
    forceA -= glm::sin(angle) * glm::length(objA.momentum()) * glm::vec3(glm::cos(momentumAngleA - angle), glm::sin(momentumAngleA - angle), 0.0f);
    forceB -= glm::sin(angle) * glm::length(objB.momentum()) * glm::vec3(glm::cos(momentumAngleB - angle), glm::sin(momentumAngleB - angle), 0.0f);
    forceA /= deltaTime;
    forceB /= deltaTime;

}

void world::calculateMovement(physObject& obj, float deltaTime) {
    glm::vec3 accel = obj.forceSum() / obj.mass;

    // possibly add a max-distance check here to prevent phasing through items.

    obj.vel += accel * deltaTime;
    obj.pos += obj.vel * deltaTime;
}

void world::addGravity(physObject& obj) {
    glm::vec3& gravForce = obj.forceVectors.add("gravForce", glm::vec3(0.0f));
    gravForce = glm::vec3(0.0f, grav, 0.0f) * obj.mass;
}


bool world::isColliding(physObject& objA, physObject& objB) {
    if(objA.getID() == objB.getID()) return false;


    if(glm::distance(objA.pos, objB.pos) <= objA.getRadius() + objB.getRadius()) {
        return true;
    }

    return false;
}

bool world::isValidForce(std::string ID) {
    if(ID == "") return false;
    if(ID == "gravForce" || ID == "External Force") return true;

    for(int i = 0; i < objectTable.size(); i++) {
        if(objectTable.getID(i) == ID) return true;
    }

    return false;
}
