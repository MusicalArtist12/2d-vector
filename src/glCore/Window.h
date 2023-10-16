#ifndef WINDOWENGINE_H
#define WINDOWENGINE_H

#include <glad/gl.h>        
#include <GLFW/glfw3.h> 

#include <imgui.h>
#include <iostream>

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
        
        int init();

        void refresh();
        void render();
        void terminate();

        ImFont* loadFont(const char* path, float size);

        int readKey(int key);
        int readMouseButton(int key);
        void readMousePos(double* x, double* y);

        void updateClock();

        bool shouldClose() { return glfwWindowShouldClose(ID); }

        window(const char* t): title(t), width(1920), height(1080), deltaTime(0.0f), lastFrame(0.0f) {}
};


#endif