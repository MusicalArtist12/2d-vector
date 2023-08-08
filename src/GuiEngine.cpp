#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <vector>

#include "WindowEngine.cpp"

struct app {
    void (*funct)();  

    app(void (*f)()) {
        funct = f;
    }

    void operator()() {
        funct();
    }
};

namespace gui {
    std::vector<app> apps;

    void init();
    void refresh();
    void render();
    void terminate();

    ImFont* loadFont(const char* path, float size);

}  

void gui::init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window::window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

}

void gui::refresh() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
}

void gui::render() {

    for(int i = 0; i < apps.size(); i++) {
        apps[i]();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void gui::terminate() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

ImFont* gui::loadFont(const char* path, float size) {
    ImGuiIO& io = ImGui::GetIO();
    return io.Fonts->AddFontFromFileTTF(path, size);
}