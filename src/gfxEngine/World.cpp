#include "gfxEngine/World.h"
#include "glCore/Window.h"
#include "glCore/Shader.h"

#include "gfxEngine/Render.h"

extern render Render;
extern window Window;

void world::addItem(physObject obj, std::string id) {
    objectTable.add(id, obj);
} 

physObject world::pullItem(std::string id) {
    return objectTable.remove(id);
}

void world::update() {
    objectNames = objectTable.nameList();
    tableSize = objectTable.size();

    for(int i = 0; i < tableSize; i++) {
        physObject& iObject = objectTable.entry(objectNames[i]);
        
        if(usePhysics) {
            calculateForces(iObject);
            calculateMovement(iObject);
        }

        Render.drawMesh(iObject.myMesh, iObject.modelMatrix());
    }
}

void world::calculateMovement(physObject& obj) {
    obj.accel = obj.forceSum / obj.mass;

    obj.vel += obj.accel * Window.deltaTime;
    obj.pos += obj.vel * Window.deltaTime;
}


void world::calculateForces(physObject& obj) {
    
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

        if(obj.pos.y < 0.0f) obj.pos.y = 0.0f;

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