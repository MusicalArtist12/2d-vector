#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <imgui.h>

#include "Objects/PhysObject.cpp"

namespace physics {
    float grav = 0.0f;

    void calculatePhysics(physObject& obj) {
        float* pos[] = { &obj.myMatrix[3][0], &obj.myMatrix[3][1] };
        float* vel[] = { &obj.vel.x, &obj.vel.y };
        float* accel[] = { &obj.accel.x, &obj.accel.y };
        
        float tmp_accel = obj.accel.y;

        if(*pos[1] >= 0) {
            obj.accel.y += grav;
        }

        obj.vel += obj.accel * window::deltaTime;

        if(*pos[1] <= 0) {
            *pos[1] = 0.0f;

            if(*vel[1] < 0) *vel[1] = 0.0f;
        }

        obj.myMatrix = glm::translate(obj.myMatrix, obj.vel * window::deltaTime);

        obj.accel.y = tmp_accel;
    }
}

