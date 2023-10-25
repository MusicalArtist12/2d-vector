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
        glm::mat4 transformMatrix; // currently does not change/ affect radius (used for ). this is a significant issue.
        mesh* myMesh;
        float radius;

    public: 
        friend class world;
        inline std::string getID() { return id; }
        inline float getRadius() { return radius; }

        glm::vec3 pos;
        glm::vec3 vel;
        glm::vec3 accel;
        
        float mass;
        bool isStatic;
        glm::vec3 forceSum();
        dictionary<glm::vec3> forceVectors;

        //bool crossesLine(glm::vec3 ptA, glm::vec3 ptB); // returns true if this object crosses a given line segment
        
        physObject(mesh* shape): 
            transformMatrix(1.0f), myMesh(shape), radius(shape->radius),
            pos(0.0f), vel(0.0f), accel(0.0f), mass(1.0f), isStatic(false), forceVectors(26) {}

        glm::mat4 modelMatrix();

};

class world {
    public: 
        float grav;
        float ground;
        float ceiling;
        bool usePhysics;

        dictionary<physObject> objectTable;   

        physObject pullItem(std::string id);

        void update();

        void calculateForces(physObject& obj);
        void calculateMovement(physObject& obj);

        float distance(glm::vec3 ptA, glm::vec3 ptB);
        bool isColliding(physObject& objA, physObject& objB);
        void resolveCollision(physObject& objA, physObject& objB);



        world():  grav(0.0f), ground(0.0f), ceiling(1000.0f), usePhysics(false), objectTable(676) {}
};




#endif