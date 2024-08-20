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

class physObject;

struct keyCallback {
    int key;
    int state;
    void (*callback)(physObject&);
};

class physObject {
    public: 
        const std::string ID;
        mesh myMesh;

        glm::vec3 pos;
        glm::vec3 vel;
        
        float mass;
        bool isStatic;

        std::vector<keyCallback> callbacks;
        dictionary<glm::vec3> forceVectors;
        
        glm::vec3 forceSum();

        inline glm::vec3 accel() { return forceSum()/mass; }
        inline glm::vec3 momentum() { return isStatic ? glm::vec3(0.0f) : mass * vel; }
        inline float kineticEnergy() { return 0.5 * mass * glm::pow(glm::length(vel), 2); }
        
        // used to determine if an object is close enough that it *could* hit
        inline float radius() { return myMesh.radius(); }

        bool isColliding(physObject& objB);
        void resolveCollision(physObject& objB, float deltaTime);
        
        void (*collisionCallback)(physObject& objA, physObject& objB, float deltaTime);

        void addKeyCallback(int key, int state, void (*callback)(physObject&));
        void inputLoop(window& Window);

        physObject(mesh shape, std::string id);

        glm::mat4 modelMatrix();
};

class world {
    private:
        void updateMovement(float deltaTime);
        void addGravity(physObject& obj);
        void calculateMovement(physObject& obj, float deltaTime);
        void updateCollisions(physObject& obj, float deltaTime);

        dictionary<physObject> objectTable;

    public:   
        float grav;
        bool usePhysics;
        int countsPerFrame = 10;

        void update(window& Window, renderQueue& queue);

        // ensure that the dictionary ID matches the object ID
        inline physObject& add(physObject value) { 
            physObject& obj = objectTable.add(value.ID, value);
            return obj; 
        }

        inline physObject remove(std::string name) { return objectTable.remove(name); }
        inline physObject& entry(std::string name) { return objectTable.entry(name); }
        inline bool hasEntry(std::string name) { return objectTable.hasEntry(name); }
        
        inline int tableSize() { return objectTable.size(); }
        inline physObject& getRef(int idx) { return objectTable.getRef(idx); }

        // used to remove forces that should not exist.
        bool isValidForce(std::string ID);
        
        world(): objectTable(676), grav(0.0f), usePhysics(false) {}
};




#endif