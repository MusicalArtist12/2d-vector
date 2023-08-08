#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "../WindowEngine.cpp"

class camera {
    public:
        glm::mat4 view();

        glm::vec2 pos = glm::vec2(0.0f, 0.0f);
        glm::mat4 projection();

        float left;
        float right;
        float bottom;
        float top;

        float relWidth;
        float relHeight;
        
        float scale = 100;

        float speed = 2.0;

        

        void InputLoop(float deltaTime);
};

glm::mat4 camera::projection() {
    /*
    return glm::perspective(
    glm::radians(FOV), 
    (float) window::width / (float) window::height, 
    0.1f, 100.0f);
    */

    relWidth = window::width/(2 * scale);
    relHeight = window::height/(2 * scale);
    
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
    
    if(glfwGetKey(window::window, GLFW_KEY_UP) == GLFW_PRESS) {
        pos += glm::vec2(0.0, speed) * deltaTime;
    }
    if(glfwGetKey(window::window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        pos -= glm::vec2(0.0, speed) * deltaTime;
    }
    if(glfwGetKey(window::window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        pos -= glm::vec2(speed, 0.0) * deltaTime;
    }
    if(glfwGetKey(window::window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        pos += glm::vec2(speed, 0.0) * deltaTime;
    }
    if(glfwGetKey(window::window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
        scale += speed * 100 * deltaTime;
    }
    if(glfwGetKey(window::window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
        scale -= speed * 100 * deltaTime;
    }

    if(glfwGetMouseButton(window::window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

        double x0, y0;

        glfwGetCursorPos(window::window, &x0, &y0);

        std::cout << "here" << std::endl;

        while(glfwGetMouseButton(window::window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

        double xC, yC;
        glfwGetCursorPos(window::window, &xC, &yC);


        double deltaX = xC - x0;
        double deltaY = yC - y0;

        pos.x += deltaX;
        pos.y += deltaY;

        std::cout << deltaX << ' ' << deltaY << std::endl;


        x0 = xC;
        y0 = yC;
        


        std::cout << "stopped" << std::endl;

    }
}