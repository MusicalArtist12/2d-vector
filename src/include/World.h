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

class PhysObject;

struct KeyCallback {
    int key;
    int state;
    void (*callback)(PhysObject&, KeyCallback* self);
    std::string name;
};

struct Polygon {
    Vertex a;
    Vertex b;
    Vertex c;
};


class PhysObject {
    public: 
        const std::string ID;
        Mesh myMesh;

        glm::vec3 pos;
        glm::vec3 scale;

        glm::vec3 vel;
        
        
        float mass;

        // completely unmoving. still has collisions
        bool isStatic;

        // no collision handling. none.
        bool isGhost;

        std::vector<KeyCallback> callbacks;
        Dictionary<glm::vec3> forceVectors;
        
        glm::vec3 forceSum();

        inline glm::vec3 accel() { return forceSum()/mass; }
        inline glm::vec3 momentum() { return isStatic ? glm::vec3(0.0f) : mass * vel; }
        inline float kineticEnergy() { return 0.5 * mass * glm::pow(glm::length(vel), 2); }
        
        // used to determine if an object is close enough that it *could* hit
        inline double radius() { return myMesh.radius(scale); }

        std::vector<Polygon> closestPolygons(PhysObject& objB);
        void transferEnergy(PhysObject& objB, float deltaTime);

        void resolveCollision(PhysObject& objB, float deltaTime);
        
        void (*collisionCallback)(PhysObject* self, PhysObject& objB, float deltaTime);


        void addKeyCallback(std::string name, int key, int state, void (*callback)(PhysObject&, KeyCallback* self));
        void removeKeyCallback(std::string name);
        void inputLoop(Window& window);

        PhysObject(Mesh shape, std::string id);

        glm::mat4 modelMatrix();
};

class World {
    private:
        void updateMovement(float deltaTime);
        void addGravity(PhysObject& obj);
        void calculateMovement(PhysObject& obj, float deltaTime);
        void updateCollisions(PhysObject& obj, float deltaTime);

        Dictionary<PhysObject> objectTable;

    public:   
        float grav;
        bool useGravity;
        bool useCollisions;
        bool isPaused;
        int countsPerFrame = 10;

        void update(Window& window, RenderQueue& queue);

        // ensure that the dictionary ID matches the object ID
        inline PhysObject& add(PhysObject value) { 
            PhysObject& obj = objectTable.add(value.ID, value);
            return obj; 
        }

        inline PhysObject remove(std::string name) { return objectTable.remove(name); }
        inline PhysObject& entry(std::string name) { return objectTable.entry(name); }
        inline bool hasEntry(std::string name) { return objectTable.hasEntry(name); }
        
        inline int tableSize() { return objectTable.size(); }
        inline PhysObject& getRef(int idx) { return objectTable.getRef(idx); }

        // used to remove forces that should not exist.
        bool isValidForce(std::string ID);
        
        World(): objectTable(676), grav(0.0f), useGravity(false), useCollisions(false), isPaused(true) {}
};




#endif