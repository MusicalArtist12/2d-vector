#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "glCore/Mesh.h"

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

        // returns whether the object's radius is within the bounds. using obj.pos.k (k == x, y) allows for the use border crossing.
        bool inXBounds(float x_min, float x_max);
        bool inYBounds(float y_min, float y_max);


    
        physObject(mesh* shape)
            : myMesh(shape) {}

        physObject(const physObject& obj)
            : myMesh(obj.myMesh), pos(obj.pos), vel(obj.vel), accel(obj.accel) {}

        glm::mat4 modelMatrix();

};

namespace physics {
    extern float grav;
    extern float ground;
    extern float ceiling;

    void calculatePhysics(physObject& obj);
}

#endif