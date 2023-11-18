#ifndef WORLDENGINE_H
#define WORLDENGINE_H

#include "Mesh.h"
#include "utils/Dictionary_impl.h"
#include "utils/Utils.h"
#include "Render.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <string>
#include <vector>

float getVecAngle(glm::vec3 vector);

class physObject {
    private: 
        std::string id; 
        
        float radius;

    public: 
        mesh* myMesh;

        glm::vec3 pos;
        glm::vec3 vel;
        glm::vec3 accel() { return forceSum()/mass; }
        float mass;
        bool isStatic;
        dictionary<glm::vec3> forceVectors;

        friend class world;
        
        glm::vec3 forceSum();
        inline std::string getID() { return id; }
        inline float getRadius() { return radius; }
        inline glm::vec3 momentum() { if(isStatic) return glm::vec3(0.0f); return mass * vel; }
        inline float kineticEnergy() { return 0.5*mass*glm::pow(glm::length(vel),2); }

        void resolveCollision(physObject& objB, float deltaTime);
        bool isColliding(physObject& objB);


        //bool crossesLine(glm::vec3 ptA, glm::vec3 ptB); // returns true if this object crosses a given line segment
        
        physObject(mesh* shape): 
            radius(shape->radius), myMesh(shape),
            pos(0.0f), vel(0.0f), mass(1.0f), isStatic(false), forceVectors(26) {}

        glm::mat4 modelMatrix();

};

class world {
    private:
        dictionary<physObject> objectTable; 

        void updateMovement(float deltaTime);

    public: 
        float grav;
        bool usePhysics;

        int countsPerFrame = 500;

        void update(float deltaTime, renderQueue& queue);

        inline physObject& add(std::string name, physObject value) { 
            physObject& obj = objectTable.add(name, value);
            obj.id = name;
            return obj; 
        }

        inline physObject remove(std::string name) { return objectTable.remove(name); }
        inline physObject& entry(std::string name) { return objectTable.entry(name); }
        inline bool hasEntry(std::string name) { return objectTable.hasEntry(name); }
        inline int tableSize() { return objectTable.size(); }
        
        inline physObject& getRef(int idx) { return objectTable.getRef(idx); }
        inline std::string getID(int idx) { return objectTable.getID(idx); }

        void addGravity(physObject& obj);
        void calculateMovement(physObject& obj, float deltaTime);
        void updateCollisions(physObject& obj, float deltaTime);

        // used to remove forces that should not exist.
        bool isValidForce(std::string ID);
        
        world(): objectTable(676), grav(0.0f), usePhysics(false) {}
        
};




#endif