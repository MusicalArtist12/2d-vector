#pragma once

#include <glad/gl.h>        
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>

#include <iostream>

namespace window {
    unsigned int width = 1920;
    unsigned int height = 1080;

    char title[] = "Title";

    GLFWwindow* window;

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    int initGLFW();
    int initWindow();
    int initOpenGL();

    int init();
 
    void refresh();
    void terminate();
}

void window::framebuffer_size_callback(GLFWwindow* window, int w, int h) {
    width = w;
    height = h;
    glViewport(0, 0, width, height);

    refresh();
}

int window::initGLFW() {
    if(glfwInit() == 0) {
        std::cout << "Failed to initalize GLFW" << std::endl;
        return 0;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return 1;
}

int window::initWindow() {
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if(window == NULL) {
        std::cout << "Failed to create Window" << std::endl;
        glfwTerminate();   

        return 0;
    }

    glfwMakeContextCurrent(window); // must come before OpenGL initialization
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    return 1;
} 

int window::initOpenGL() {
    if (gladLoadGL(glfwGetProcAddress) == 0) {
        std::cout << "Failed to create OpenGL context" <<std::endl;
        return 0;
    }
    
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return 1;
}

int window::init() {
    initGLFW();
    initWindow();
    initOpenGL();

    return 1;
}

void window::refresh() {
    glfwSwapBuffers(window);
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

}

void window::terminate() {

    glfwTerminate();
}