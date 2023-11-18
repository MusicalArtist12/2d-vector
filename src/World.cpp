#include "World.h"
#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

extern world World;

float getVecAngle(glm::vec3 vector) {
    if(glm::length(vector) == 0.0) return 0.0f;
    
    float angle1;

    if(vector.x > 0) angle1 = glm::atan(vector.y/vector.x);
    else if(vector.x < 0) angle1 = glm::atan(vector.y/vector.x) - glm::radians(180.0f);
    else angle1 = glm::radians(90.0f) * vector.y/glm::abs(vector.y);

    return angle1;
}

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

void physObject::resolveCollision(physObject& objB, float deltaTime) {
    glm::vec3& forceA = forceVectors.add( objB.ID, glm::vec3(0.0f) );

    glm::vec3 AB = objB.pos - pos;

    float angle1 = getVecAngle(AB);
    float angleAp = getVecAngle(momentum()) - angle1;
    float angleBp = getVecAngle(objB.momentum()) - angle1;
    float angleAf = getVecAngle(forceSum()) - angle1;

    glm::vec3 f_effA = glm::normalize(AB) * glm::length((forceSum())) * glm::cos(angleAf);
    glm::vec3 p_effB = glm::normalize(AB) * glm::length(objB.momentum()) * glm::cos(angleBp);
    glm::vec3 p_effA = glm::normalize(AB) * glm::length(momentum()) * glm::cos(angleAp);

    forceA = (p_effB - p_effA)/deltaTime - f_effA;
}

bool physObject::isColliding(physObject& objB) {
    if(ID == objB.ID) return false;

    if(glm::distance(pos, objB.pos) <= radius() + objB.radius()) {
        return true;
    }

    return false;
}

physObject::physObject(mesh shape, std::string id):
    ID(id), myMesh(shape),
    pos(0.0f), vel(0.0f), mass(1.0f), isStatic(false), forceVectors(26) {
        for(int i = 0; i < vertices.size(); i++) {
            vertices.push_back(&myMesh.vertices[i]);
        }
}

// ******************************************************************

void world::update(float deltaTime, renderQueue& RenderQueue) {

    if(deltaTime < 0) deltaTime = 0;
    for(int i = 0; i < countsPerFrame; i++) {
        updateMovement(deltaTime/(float)(countsPerFrame));
    }

    for(int i = 0; i < objectTable.size(); i++) {
        physObject& iObj = objectTable.getRef(i);
        RenderQueue.queue.push_back(drawData(&iObj.myMesh, iObj.modelMatrix()));
    }
}

void world::updateMovement(float deltaTime) {
    for(int i = 0; i < objectTable.size(); i++) {

        physObject& iObject = objectTable.getRef(i);
        
        if(usePhysics) updateCollisions(iObject, deltaTime);
    }

    for(int i = 0; i < objectTable.size(); i++) {
        physObject& iObject = objectTable.getRef(i);
        
        if(usePhysics && !iObject.isStatic) calculateMovement(iObject, deltaTime);  
    }
}

void world::updateCollisions(physObject& obj, float deltaTime) {
    for(int j = 0; j < objectTable.size(); j++) {
        physObject& iObj = objectTable.getRef(j);

        if(obj.isColliding(iObj)) {
            if( !obj.forceVectors.hasEntry(iObj.ID) || !iObj.forceVectors.hasEntry(obj.ID) ){
                obj.resolveCollision(iObj, deltaTime);
                iObj.resolveCollision(obj, deltaTime);
            }
        } 

        else {
            if( obj.forceVectors.hasEntry(iObj.ID) ) obj.forceVectors.remove(iObj.ID);
            if( iObj.forceVectors.hasEntry(obj.ID) ) iObj.forceVectors.remove(obj.ID);
        }
    }  
}



void world::calculateMovement(physObject& obj, float deltaTime) {
    addGravity(obj);
    
    glm::vec3 accel = obj.forceSum() / obj.mass;

    // possibly add a max-distance check here to prevent phasing through items.

    obj.vel += accel * deltaTime;
    obj.pos += obj.vel * deltaTime;
}

void world::addGravity(physObject& obj) {
    glm::vec3& gravForce = obj.forceVectors.add("gravForce", glm::vec3(0.0f));
    gravForce = glm::vec3(0.0f, grav, 0.0f) * obj.mass;
}

bool world::isValidForce(std::string ID) {
    if(ID == "") return false;
    if(ID == "gravForce" || ID == "External Force") return true;

    for(int i = 0; i < objectTable.size(); i++) {
        if(objectTable.getID(i) == ID) return true;
    }

    return false;
}
