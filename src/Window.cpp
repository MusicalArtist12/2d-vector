#include "Window.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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
    ID = glfwCreateWindow(width, height, title, NULL, NULL);
    if(ID == NULL) {
        std::cout << "Failed to create Window" << std::endl;
        glfwTerminate();   

        return 0;
    }

    glfwMakeContextCurrent(ID); // must come before OpenGL initialization
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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
    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(ID, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    return 1;
}

window::window(const char* t): title(t), width(1920), height(1080), deltaTime(0.0f), lastFrame(0.0f) {
    initGLFW();
    initWindow();
    initOpenGL();
    initImGui();

}

void window::refresh() {
    glfwSwapBuffers(ID);
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    glfwGetFramebufferSize(ID, &width, &height);
    glViewport(0, 0, width, height);
}

void window::render() {
    updateClock();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

    return glfwGetKey(ID, key);
}

int window::readMouseButton(int key) {
    ImGuiIO& io = ImGui::GetIO();
    if(io.WantCaptureMouse) {
        return 0;
    } 

    return glfwGetMouseButton(ID, key);
}

void window::readMousePos(double* x, double* y) {
    ImGuiIO& io = ImGui::GetIO();
    if(io.WantCaptureMouse) {
        return;
    } 

    glfwGetCursorPos(ID, x, y);
}

void window::updateClock() {
    float currentFrame = glfwGetTime(); // glfwGetTIme returns seconds
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;  
}

