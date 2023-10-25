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
    private: 
        std::string id; 
    public: 
        friend class world;
        inline std::string getID() { return id; }

        mesh* myMesh;
        
        glm::vec3 pos = glm::vec3(0.0f);
        glm::vec3 vel = glm::vec3(0.0f);
        glm::vec3 accel = glm::vec3(0.0f);
        
        glm::mat4 transformMatrix = glm::mat4(1.0f);

        float mass;

        glm::vec3 forceSum = glm::vec3(0.0f);
        dictionary<glm::vec3> forceVectors;

        bool isStatic;

        bool xBounds(float x_min, float x_max);
        bool yBounds(float y_min, float y_max);

        /*  returns 1 if the entire object is to the right of the axis
            returns 0 if the object crosses the axis
            returns -1 if the entire object is to the left of the axis
        */
        int xAxisRelation(float axis);
        int yAxisRelation(float axis);

        float distanceFrom(glm::vec3 pos);
        
        // only moves the parent object. call on both. 
        void calculateCollision(physObject& obj);

        physObject(mesh* shape)
            : myMesh(shape), mass(1.0f), forceVectors(26), isStatic(false) {}

        glm::mat4 modelMatrix();

};

class world {
    public: 
        float grav;
        float ground;
        float ceiling;
        bool usePhysics;

        dictionary<physObject> objectTable;   

        void addItem(physObject obj, std::string id);
        physObject pullItem(std::string id);

        void update();

        void calculateForces(physObject& obj);
        void calculateMovement(physObject& obj);

        world():  grav(0.0f), ground(0.0f), ceiling(1000.0f), usePhysics(false), objectTable(676) {}
};

#endif