#ifndef WORLDENGINE_H
#define WORLDENGINE_H

#include "Mesh.h"
#include "utils/Dictionary_impl.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <string>
#include <vector>

class physObject {
    public: 
        mesh* myMesh;

        glm::vec3 pos = glm::vec3(0.0f);
        glm::vec3 vel = glm::vec3(0.0f);
        glm::vec3 accel = glm::vec3(0.0f);

        float mass;

        glm::vec3 forceSum = glm::vec3(0.0f);

        dictionary<glm::vec3> forceVectors;

        bool xBounds(float x_min, float x_max);
        bool yBounds(float y_min, float y_max);

        /*  returns 1 if the entire object is to the right of the axis
            returns 0 if the object crosses the axis
            returns -1 if the entire object is to the left of the axis
        */
        int xAxisRelation(float axis);
        int yAxisRelation(float axis);

        physObject(mesh* shape)
            : myMesh(shape), mass(1.0f), forceVectors(26) {}

        glm::mat4 modelMatrix();

};

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