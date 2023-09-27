#include "gfxEngine/WorldEngine.h"

#include "glCore/Shader.h"

#include "physicsEngine/PhysicsEngine.h"
#include "gfxEngine/RenderEngine.h"
#include "utils/Dictionary.h"

namespace world {
    dictionary<physObject> objectTable;   
    std::string* objectNames;
    int tableSize;
    bool usePhysics = false;
}

void world::addItem(physObject obj, std::string id) {
    objectTable.addItem(obj, id);
} 

void world::update() {
    objectNames = objectTable.getArrayOfItems();
    tableSize = objectTable.size();

    for(int i = 0; i < tableSize; i++) {
        physObject& iObject = objectTable.getItem(objectNames[i]);
        
        if(usePhysics) {
            physics::calculatePhysics(iObject);
        }

        if(!iObject.myMesh->generated) {
            render::generateBuffer(iObject.myMesh);
        }

        render::drawMesh(iObject.myMesh, iObject.myMatrix);
    }
}