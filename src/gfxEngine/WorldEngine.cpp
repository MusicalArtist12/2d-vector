#include "gfxEngine/WorldEngine.h"

#include "glCore/Shader.h"

#include "physicsEngine/PhysicsEngine.h"
#include "gfxEngine/RenderEngine.h"

namespace world {
    dictionary<physObject> objectTable(676);   
    std::vector<std::string> objectNames;
    
    int tableSize;
    bool usePhysics = false;
}

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
            physics::calculateForces(iObject);
            physics::calculateMovement(iObject);
        }

        render::drawMesh(iObject.myMesh, iObject.modelMatrix());
    }
}