#include "glCore/WindowEngine.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace window {
    unsigned int width = 1920;
    unsigned int height = 1080;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    char title[] = "Title";

    GLFWwindow* window;

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
        ImGui::Text("Clock Rate: %3f", deltaTime);
        ImGui::Text("Height: %3u", height);
        ImGui::Text("Width: %3u", width);

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

void window::updateClock() {
    float currentFrame = glfwGetTime(); // glfwGetTIme returns seconds
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;  
}