#pragma once

#include <glad/gl.h>     
#include <GLFW/glfw3.h>  

namespace physics {
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    void updateClock() {
        float currentFrame = glfwGetTime(); // glfwGetTIme returns seconds
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  
    }
}
