#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <imgui.h>

class camera {
    private:
        const float maxScale = 10000;
        const float minScale = 1.00;

    public:
        float left;
        float right;
        float bottom;
        float top;
        
        float scale;
        float speed;

        glm::vec2 pos;
        
        glm::mat4 view();
        glm::mat4 projection();

        void InputLoop(float deltaTime);
        
        camera(): scale(100), speed(1000.0), pos(0.0f, 0.0f) {}

};

#endif