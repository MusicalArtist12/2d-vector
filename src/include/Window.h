#ifndef WINDOWENGINE_H
#define WINDOWENGINE_H

#include <glad/gl.h>        
#include <GLFW/glfw3.h> 

#include <imgui.h>
#include <iostream>

#include "Camera.h"

class window {
    private:
        int initGLFW();
        int initWindow();
        int initOpenGL();
        int initImGui();

        const char* title;

        GLFWwindow* ID;

    public:
        int width;
        int height;
        float deltaTime;
        float lastFrame;

        camera myCamera;

        glm::vec3 clearColor = glm::vec3((102.0f/255.0f), (180.0f/255.0f), (222.0f/255.0f));

        void refresh();
        void render();
        void terminate();

        ImFont* loadFont(const char* path, float size);

        int readKey(int key);
        int readMouseButton(int key);
        void readMousePos(double* x, double* y);

        void updateClock();

        bool shouldClose() { return glfwWindowShouldClose(ID); }

        window(const char* t);
};


#endif