#include "gfxEngine/WorldEngine.h"

#include "glCore/Shader.h"

#include "physicsEngine/PhysicsEngine.h"
#include "gfxEngine/RenderEngine.h"

namespace world {
    dictionary<physObject> objectTable(676);   
    std::string* objectNames;
    
    int tableSize;
    bool usePhysics = false;
}

void world::addItem(physObject obj, std::string id) {
    objectTable.addItem(obj, id);
} 

physObject world::pullItem(std::string id) {
    return objectTable.pullItem(id);
}

void world::update() {
    objectNames = objectTable.getArrayOfIDs();
    tableSize = objectTable.size();

    for(int i = 0; i < tableSize; i++) {
        physObject& iObject = objectTable.getItem(objectNames[i]);
        
        if(usePhysics) {
            physics::calculatePhysics(iObject);
        }

        render::drawMesh(iObject.myMesh, iObject.modelMatrix());
    }
}