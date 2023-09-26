#pragma once

#include <glad/gl.h>        
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

namespace window {
    unsigned int width = 1920;
    unsigned int height = 1080;

    char title[] = "Title";

    GLFWwindow* window;

    // callbacks
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    // initializers
    int initGLFW();
    int initWindow();
    int initOpenGL();
    int initImGui();
    int init();

    void appInfo();
 
    void refresh();
    void render();
    void terminate();

    // io calls
    ImFont* loadFont(const char* path, float size);
    int readKey(int key);
    int readMouseButton(int key);
    void readMousePos(double* x, double* y);

    // runtime 
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    void updateClock() {
        float currentFrame = glfwGetTime(); // glfwGetTIme returns seconds
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  
    }
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

int window::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window::window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    return 1;
}

int window::init() {
    initGLFW();
    initWindow();
    initOpenGL();
    initImGui();

    return 1;
}

void window::refresh() {
    glfwSwapBuffers(window);
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
}

void window::render() {
    updateClock();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void window::appInfo() {
    static bool on = true;
    ImGui::Begin("Window Info", &on);

        std::string cl = "ClockRate: " + std::to_string(deltaTime);
        std::string wh = "Window Height:" + std::to_string(window::height);
        std::string ww = "Window Width: " + std::to_string(window::width);

        ImGui::Text(cl.c_str());
        ImGui::Text(ww.c_str());
        ImGui::Text(wh.c_str());

    ImGui::End();
}

void window::terminate() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

ImFont* window::loadFont(const char* path, float size) {
    ImGuiIO& io = ImGui::GetIO();
    return io.Fonts->AddFontFromFileTTF(path, size);
}

int window::readKey(int key) {
    ImGuiIO& io = ImGui::GetIO();
    if(io.WantCaptureKeyboard) {
        return 0;
    }

    return glfwGetKey(window, key);
}

int window::readMouseButton(int key) {
    ImGuiIO& io = ImGui::GetIO();
    if(io.WantCaptureMouse) {
        return 0;
    } 

    return glfwGetMouseButton(window, key);
}

void window::readMousePos(double* x, double* y) {
    ImGuiIO& io = ImGui::GetIO();
    if(io.WantCaptureMouse) {
        return;
    } 

    glfwGetCursorPos(window::window, x, y);
}