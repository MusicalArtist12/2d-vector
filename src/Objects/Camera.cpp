#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "../WindowEngine.cpp"

#include <imgui.h>

class camera {
    public:
        float left;
        float right;
        float bottom;
        float top;
        
        float scale = 100;

        float speed = 1000.0;

        const float maxScale = 10000;
        const float minScale = 0.100;

        glm::mat4 view();

        glm::vec2 pos = glm::vec2(0.0f, 0.0f);
        glm::mat4 projection();

        void InputLoop(float deltaTime);
        void appInfo();
};

glm::mat4 camera::projection() {
    /*
    return glm::perspective(
    glm::radians(FOV), 
    (float) window::width / (float) window::height, 
    0.1f, 100.0f);
    */

    float relWidth = window::width/(2 * scale);
    float relHeight = window::height/(2 * scale);
    
    left = -1 * relWidth;
    right = 1* relWidth;
    bottom = -1 * relHeight;
    top = 1 * relHeight;

    return glm::ortho(left, right, bottom, top, 0.0f, 100.0f);

    //return glm::perspective(10.0f, (float)window::width/window::height, 10.0f, 200.0f);
    
}

glm::mat4 camera::view() {
    glm::mat4 view = glm::lookAt(glm::vec3(pos, 1.0f), glm::vec3(pos, 1.0f)+glm::vec3(0.0, 0.0, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    return view;
}

void camera::InputLoop(float deltaTime) {
    
    if(window::readKey(GLFW_KEY_UP) == GLFW_PRESS) {
        pos += glm::vec2(0.0, speed/scale) * deltaTime;
    }
    if(window::readKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
        pos -= glm::vec2(0.0, speed/scale) * deltaTime;
    }
    if(window::readKey(GLFW_KEY_LEFT) == GLFW_PRESS) {
        pos -= glm::vec2(speed/scale, 0.0) * deltaTime;
    }
    if(window::readKey(GLFW_KEY_RIGHT) == GLFW_PRESS) {
        pos += glm::vec2(speed/scale, 0.0) * deltaTime;
    }
    if(window::readKey(GLFW_KEY_PAGE_UP) == GLFW_PRESS && scale < maxScale) {
        scale += speed * deltaTime;

        if(scale > maxScale) {
            scale = maxScale;
        }
    }
    if(window::readKey(GLFW_KEY_PAGE_DOWN) == GLFW_PRESS && scale > minScale) {
        scale -= speed * deltaTime;

        if(scale < minScale) {
            scale = minScale;
        }
    }

    static bool next_cycle = false;
    static double x_i, y_i;
    if(next_cycle) {        
        double x_f, y_f;

        window::readMousePos(&x_f, &y_f);

        double deltaX = (x_f - x_i) * (1/scale);
        double deltaY = (y_f - y_i) * (1/scale);

        pos.x -= deltaX;
        pos.y += deltaY;
    }


    if(window::readMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        next_cycle = true;
        window::readMousePos(&x_i, &y_i);
    } else {
        next_cycle = false;
    }    
}

void camera::appInfo() {
    static bool on = true;
    ImGui::Begin("Camera Info", &on);

        std::string cp = "Camera Pos: (" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ")";

        ImGui::Text(cp.c_str());

        ImGui::SliderFloat("Camera Scale", &scale, minScale, maxScale);
        ImGui::SliderFloat("Camera Speed", &speed, 500, 2000);

    ImGui::End();
}
