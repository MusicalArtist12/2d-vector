#ifndef WINDOWENGINE_H
#define WINDOWENGINE_H

#include <glad/gl.h>        
#include <GLFW/glfw3.h> 

#include <imgui.h>
#include <iostream>

namespace window {
    extern unsigned int width;
    extern unsigned int height;
    extern float deltaTime;
    extern float lastFrame;
    extern char title[];
    extern GLFWwindow* window;

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    int initGLFW();
    int initWindow();
    int initOpenGL();
    int initImGui();
    int init();

    void refresh();
    void render();
    void terminate();

    ImFont* loadFont(const char* path, float size);
    int readKey(int key);
    int readMouseButton(int key);
    void readMousePos(double* x, double* y);

    void updateClock();

    #ifdef WINDOW_APP
        void appInfo();
    #endif
}

#endif