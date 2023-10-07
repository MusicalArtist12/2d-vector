
#include "gfxEngine/Camera.h"
#include "glCore/WindowEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <imgui.h>


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
        scale += (speed * scale * 0.001) * deltaTime;

        if(scale > maxScale) {
            scale = maxScale;
        }

    }
    if(window::readKey(GLFW_KEY_PAGE_DOWN) == GLFW_PRESS && scale > minScale) {
        scale -= (speed * scale * 0.001) * deltaTime;

        if(scale < minScale) {
            scale = minScale;
        }

    }

    static bool next_cycle = false;
    static double x_i, y_i;
    if(next_cycle) {        
        double x_f, y_f;

        window::readMousePos(&x_f, &y_f);

        double deltaX = (x_f - x_i) / scale; // window width/height cancel out
        double deltaY = (y_f - y_i) / scale;

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

void camera::getMousePositionRelative() {
    double x, y;
    window::readMousePos(&x, &y);

    //double x_pos = (x / scale);
    //double y_pos = (y / scale);

    //std::cout << "MOUSE x: " << (x_pos - window::width/(2 * scale)) + pos.x << std::endl;
    //std::cout << "MOUSE y: " << pos.y - (y_pos - window::height/(2 * scale)) << std::endl;
}

