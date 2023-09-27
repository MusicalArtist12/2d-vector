#ifndef PHYSOBJ_H
#define PHYSOBJ_H

#include "glCore/Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <string>
#include <vector>

class physObject {
    public: 
        mesh* myMesh;
        glm::mat4 myMatrix = glm::mat4(1.0f);

        glm::vec3 vel = glm::vec3(0.0f);
        glm::vec3 accel = glm::vec3(0.0f);
        
        physObject(mesh* shape)
            : myMesh(shape) {}

        physObject(const physObject& obj)
            : myMesh(obj.myMesh), myMatrix(obj.myMatrix), vel(obj.vel), accel(obj.accel) {}
};

#endif