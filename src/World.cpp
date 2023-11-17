#include "World.h"
#include "Render.h"

#include "Shader.h"

#include <set>

extern render Render;
extern world World;

glm::mat4 physObject::modelMatrix() {
    glm::mat4 model = glm::mat4(1.0f);

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

    if(deltaTime < 0) deltaTime = 0;
    for(int i = 0; i < countsPerFrame; i++) {
        updateMovement(deltaTime/(float)(countsPerFrame));
    }

    for(int i = 0; i < objectTable.size(); i++) {
        physObject& iObject = objectTable.getRef(i);
        Render.renderQueue.push_back(drawData(iObject.myMesh, iObject.modelMatrix()));

    }

}


void world::updateMovement(float deltaTime) {
    for(int i = 0; i < objectTable.size(); i++) {

        physObject& iObject = objectTable.getRef(i);
        
        addGravity(iObject);

        for(int j = 0; j < objectTable.size(); j++) {
            physObject& jObject = objectTable.getRef(j);

            if(isColliding(iObject, jObject)) {
                if( !iObject.forceVectors.hasEntry(jObject.getID()) || !jObject.forceVectors.hasEntry(iObject.getID()) ){
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

float getVecAngle(glm::vec3 vector) {
    if(glm::length(vector) == 0.0) return 0.0f;
    
    float angle1;

    if(vector.x > 0) angle1 = glm::atan(vector.y/vector.x);
    else if(vector.x < 0) angle1 = glm::atan(vector.y/vector.x) - glm::radians(180.0f);
    else angle1 = glm::radians(90.0f) * vector.y/glm::abs(vector.y);

    return angle1;
}

void world::resolveCollision(physObject& objA, physObject& objB, float deltaTime) {
    
    glm::vec3& forceA = objA.forceVectors.add( objB.getID(), glm::vec3(0.0f) );
    glm::vec3& forceB = objB.forceVectors.add( objA.getID(), glm::vec3(0.0f) );

    glm::vec3 AB = objB.pos - objA.pos;

    float angle1 = getVecAngle(AB);
    float angleAp = getVecAngle(objA.momentum()) - angle1;
    float angleBp = getVecAngle(objB.momentum()) - angle1;

    float angleAf = getVecAngle(objA.forceSum()) - angle1;
    float angleBf = getVecAngle(objB.forceSum()) - angle1;

    glm::vec3 p_effA = glm::normalize(AB) * glm::length(objA.momentum()) * glm::cos(angleAp);
    glm::vec3 f_effA = glm::normalize(AB) * glm::length((objA.forceSum())) * glm::cos(angleAf);
    
    glm::vec3 p_effB = glm::normalize(AB) * glm::length(objB.momentum()) * glm::cos(angleBp);
    glm::vec3 f_effB = glm::normalize(AB) * glm::length((objB.forceSum())) * glm::cos(angleBf);

    forceA = (p_effB - p_effA)/deltaTime - f_effA;
    forceB = (p_effA - p_effB)/deltaTime - f_effB;

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
