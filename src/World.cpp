#include "World.h"
#include "Render.h"

#include "Window.h"
#include "Shader.h"

extern render Render;
extern window Window;

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
    glm::mat4 model = transformMatrix;

    model = glm::translate(glm::mat4(1.0f), pos) * model;

    return model;
}

float physObject::distanceFrom(glm::vec3 point) {
    glm::vec3 delta = glm::vec3(point.x - pos.x, point.y - pos.y, point.z - pos.z);

    return (float)sqrt(pow(delta.x,2) + pow(delta.y,2));
}

void physObject::calculateCollision(physObject& obj) {
    float distance = distanceFrom(obj.pos);
    

    if(distance > myMesh->radius) return;

    //forceVectors.add(obj.getID(), )
}

// ******************************************************************

void world::addItem(physObject obj, std::string id) {
    objectTable.add(id, obj).id = id;

} 

physObject world::pullItem(std::string id) {
    return objectTable.remove(id);
}

void world::update() {

    for(int i = 0; i < objectTable.size(); i++) {
        physObject& iObject = objectTable.getRef(i);
        iObject.id = objectTable.getID(i);

        if(usePhysics && !iObject.isStatic) {
            calculateMovement(iObject);
        }

        Render.drawMesh(iObject.myMesh, iObject.modelMatrix());
    }

    for(int i = 0; i < objectTable.size(); i++) {
        physObject& iObject = objectTable.getRef(i);

        if(usePhysics && !iObject.isStatic) {
            calculateForces(iObject);
        }   
        
    }
}

void world::calculateMovement(physObject& obj) {
    obj.accel = obj.forceSum / obj.mass;

    obj.vel += obj.accel * Window.deltaTime;
    obj.pos += obj.vel * Window.deltaTime;
}

void world::calculateForces(physObject& obj) {
    
    // generate default forces if they don't exist
    glm::vec3& normalFloor = obj.forceVectors.add("physEngine normalFloor", glm::vec3(0.0f));
    glm::vec3& gravForce = obj.forceVectors.add("physEngine gravForce", glm::vec3(0.0f));

    gravForce = glm::vec3(0.0f, grav, 0.0f) * obj.mass;

    if(obj.yAxisRelation(ground) <= 0) {
        if(gravForce.y <= 0) 
            normalFloor = gravForce * -1.0f;
        
        if(obj.vel.y - obj.myMesh->radius < 0.0f) obj.vel.y = 0.0f;
        if(obj.pos.y - obj.myMesh->radius < 0.0f) obj.pos.y = obj.myMesh->radius;

    } else {
        normalFloor = glm::vec3(0.0f);
    }

    obj.accel = glm::vec3(0.0f);
    obj.forceSum = glm::vec3(0.0f);

    for(int i = 0; i < obj.forceVectors.size(); i++) {
        obj.forceSum += obj.forceVectors.getRef(i);
    }
}
