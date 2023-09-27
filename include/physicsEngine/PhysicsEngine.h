#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "physicsEngine/PhysObject.h"

namespace physics {
    extern float grav;

    void calculatePhysics(physObject& obj);
}

#endif