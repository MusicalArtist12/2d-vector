#ifndef WORLDENGINE_H
#define WORLDENGINE_H

#include "Mesh.h"
#include "glm/ext/scalar_constants.hpp"
#include "utils/Dictionary_impl.h"
#include "utils/Utils.h"
#include "Render.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>

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
    glm::vec3 a;
    glm::vec3 b;
    glm::vec3 c;

    inline glm::vec3& index(int i) {
        i = i % 3;
        
        if (i == 0) {
            return a;
        }
        if (i == 1) {
            return b;
        }

        return c;
    }

    // modulates i
    inline glm::vec3& operator[](int i) {
        return index(i);
    }

    inline double height(int i) {
        return glm::sin(angle(i + 1)) * glm::distance(index(i), index(i + 1));
    }

    inline double angle(int i) {
        glm::vec3 lineA = index(i + 1) - index(i);
        glm::vec3 lineB = index(i + 2) - index(i);
        
        double angleA = glm::atan(lineA.y, lineA.x);
        double angleB = glm::atan(lineB.y, lineB.x);

        double myAngle = angleA < angleB ? angleB - angleA : angleA - angleB;

        if (myAngle > glm::radians(180.0)) {
            myAngle = glm::radians(360.0) - myAngle; 
        }

        return myAngle;
    }

    inline double length(int x, int y) {
        return glm::distance(index(x), index(y));
    }
};


class PhysObject {
    public: 
        const std::string ID;
        Mesh myMesh;

        glm::vec3 pos;
        glm::vec3 scale;
        
        // radians
        float angle; 

        glm::vec3 vel;
        
        float mass;

        // counters every force applied to it
        bool isStatic;

        // no collision handling. none.
        bool isGhost;

        std::vector<KeyCallback> callbacks;
        Dictionary<glm::vec3> forceVectors;
        
        glm::vec3 forceSum();

        inline glm::vec3 accel() { return forceSum()/mass; }
        inline glm::vec3 momentum() { return mass * vel; }
        inline float kineticEnergy() { return 0.5 * mass * glm::pow(glm::length(vel), 2); }
        
        inline void resetForces() {
            for (int i = 0; i < forceVectors.size(); i++) {
                forceVectors.remove(forceVectors.getID(i));
            }
        }

        // used to determine if an object is close enough that it *could* hit
        inline double radius() { return myMesh.radius(scale); }

        std::vector<Polygon> closestPolygons(PhysObject& objB);
        
        void (*preCollisionCallback)(PhysObject* self, PhysObject& objB, glm::vec3 collisionPoint, float deltaTime);
        void (*postCollisionCallback)(PhysObject* self, PhysObject& objB, glm::vec3 collisionPoint, float deltaTime);


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
        void updateCollisions(float deltaTime);
        std::vector<glm::vec3> collisionPoints(Polygon polyA, Polygon polyB, bool& isClipping);

        Dictionary<PhysObject> objectTable;

    public:   
        float grav;
        bool useGravity;
        bool useCollisions;
        bool isPaused;
        int countsPerFrame = 10;
        int timeScalePercent = 100;

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