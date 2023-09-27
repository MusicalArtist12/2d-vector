#ifndef WORLDENGINE_H
#define WORLDENGINE_H

#include "utils/Dictionary_impl.h"
#include "physicsEngine/PhysObject.h"

#include <string>

namespace world {
    extern dictionary<physObject> objectTable;   
    extern std::string* objectNames;
    extern int tableSize;
    extern bool usePhysics;
    
    void addItem(physObject obj, std::string id);

    void update();
};

#endif