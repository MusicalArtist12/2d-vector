#pragma once

#include "dictionary.cpp"

#include "Objects/Shader.cpp"
#include "Objects/Mesh.cpp"

#include "Objects/PhysObject.cpp"

#include "PhysicsEngine.cpp"
#include "RenderEngine.cpp"

namespace world {
    dictionary<physObject> objectTable;   
    std::string* objectNames;
    int tableSize;

    bool usePhysics = false;

    void addItem(physObject obj, std::string id) {
        objectTable.addItem(obj, id);
    } 

    void update();
};

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