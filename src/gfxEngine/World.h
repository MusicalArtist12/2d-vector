#ifndef WORLDENGINE_H
#define WORLDENGINE_H

#include "utils/Dictionary_impl.h"
#include "physObject.cpp"

#include <imgui.h>
#include <string>

class world {
    public: 
        float grav;
        float ground;
        float ceiling;
        int tableSize;
        bool usePhysics;

        dictionary<physObject> objectTable;   
        std::vector<std::string> objectNames;

        void addItem(physObject obj, std::string id);

        physObject pullItem(std::string id);

        void update();

        void calculateForces(physObject& obj);
        void calculateMovement(physObject& obj);

        world():  grav(0.0f), ground(0.0f), ceiling(1000.0f), usePhysics(false), objectTable(676) {}
};

#endif