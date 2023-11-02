#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <imgui.h>

glm::mat4 camera::projection(window& Window) {
    float relWidth = Window.width/(2 * scale);
    float relHeight = Window.height/(2 * scale);
    
    float left = -1 * relWidth;
    float right = 1* relWidth;
    float bottom = -1 * relHeight;
    float top = 1 * relHeight;

    return glm::ortho(left, right, bottom, top, 0.0f, 100.0f);
}

glm::mat4 camera::view() {
    glm::mat4 view = glm::lookAt(glm::vec3(pos, 1.0f), glm::vec3(pos, 1.0f)+glm::vec3(0.0, 0.0, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    return view;
}

void camera::InputLoop(window& Window) {
    
    if(Window.readKey(GLFW_KEY_UP) == GLFW_PRESS) {
        pos += glm::vec2(0.0, speed/scale) * Window.deltaTime;
    }
    if(Window.readKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
        pos -= glm::vec2(0.0, speed/scale) * Window.deltaTime;
    }
    if(Window.readKey(GLFW_KEY_LEFT) == GLFW_PRESS) {
        pos -= glm::vec2(speed/scale, 0.0) * Window.deltaTime;
    }
    if(Window.readKey(GLFW_KEY_RIGHT) == GLFW_PRESS) {
        pos += glm::vec2(speed/scale, 0.0) * Window.deltaTime;
    }
    if(Window.readKey(GLFW_KEY_PAGE_UP) == GLFW_PRESS && scale < maxScale) {
        scale += (speed * scale * 0.001) * Window.deltaTime;

        if(scale > maxScale) {
            scale = maxScale;
        }

    }
    if(Window.readKey(GLFW_KEY_PAGE_DOWN) == GLFW_PRESS && scale > minScale) {
        scale -= (speed * scale * 0.001) * Window.deltaTime;

        if(scale < minScale) {
            scale = minScale;
        }

    }

    static bool next_cycle = false;
    static double x_i, y_i;
    if(next_cycle) {        
        double x_f, y_f;

        Window.readMousePos(&x_f, &y_f);

        double deltaX = (x_f - x_i) / scale; // window width/height cancel out
        double deltaY = (y_f - y_i) / scale;

        pos.x -= deltaX;
        pos.y += deltaY;
    }


    if(Window.readMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        next_cycle = true;
        Window.readMousePos(&x_i, &y_i);
    } else {
        next_cycle = false;
    }    
}
