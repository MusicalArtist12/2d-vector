#include "physicsEngine/PhysicsEngine.h"
#include "glCore/WindowEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace physics {
    float grav = 0.0f;
}

void physics::calculatePhysics(physObject& obj) {
    float* vel[] = { &obj.vel.x, &obj.vel.y };
    //float* accel[] = { &obj.accel.x, &obj.accel.y };
    
    float tmp_accel = obj.accel.y;

    if(obj.pos[1] >= 0) {
        obj.accel.y += grav;
    }

    obj.vel += obj.accel * window::deltaTime;

    if(obj.pos[1] <= 0) {
        obj.pos[1] = 0.0f;

        if(*vel[1] < 0) *vel[1] = 0.0f;
    }

    obj.pos += (obj.vel * window::deltaTime);

    obj.accel.y = tmp_accel;
}

