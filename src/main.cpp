#define CAMERA_APP 1
#define WINDOW_APP 1
#define WORLD_APP 1

#include <iostream>
#include <iomanip>
#include <string>

#include <glad/gl.h>     
#include <GLFW/glfw3.h>  

#include "glCore/WindowEngine.h" // Layer between OS and code
#include "gfxEngine/RenderEngine.h"
#include "physicsEngine/PhysicsEngine.h"
#include "gfxEngine/WorldEngine.h"

std::vector<vertex> mesh0_vertices = {
    vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
    vertex(-1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
    vertex(1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
    vertex(1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f)
};

std::vector<GLuint> mesh0_index = {
    0, 1, 2,
    0, 3, 2
};

void loop();

int main() { 
    mesh mesh0(mesh0_vertices, mesh0_index);
    physObject myObject(&mesh0);

    world::addItem(myObject, "rainbow cube");

    window::init();
    window::loadFont("data/fonts/SourceCodePro-Regular.otf", 32);

    render::activeShader = new shader("data/shaders/gen.vert", "data/shaders/gen.frag");
    render::activeCamera = new camera;
    
    loop();

    window::terminate();
    return 0;
}

glm::vec3 background_color(0.0f, 0.0f, 0.0f);

void loop() {
    
    while(!glfwWindowShouldClose(window::window)) {
        window::refresh();
        ImGui::NewFrame();

        glClearColor(background_color[0], background_color[1], background_color[2], 1.0f);  
        
        render::updateCamera();
        render::activeCamera->InputLoop(window::deltaTime);
        render::activeCamera->getMousePositionRelative();

        world::update();
        
        render::activeCamera->appInfo();
        window::appInfo();
        world::appInfo();

        window::render();
        
    }
}

void camera::appInfo() {
    static bool on = true;

    ImGui::Begin("Camera Info", &on);

    float curPos[] = {pos[0], pos[1]};

    ImGui::SliderFloat2("Camera Position", curPos, -1000, 1000);
    ImGui::SliderFloat("Camera Scale", &scale, minScale, maxScale);
    ImGui::SliderFloat("Camera Speed", &speed, 500, 2000);

    pos[0] = curPos[0];
    pos[1] = curPos[1];

    ImGui::End();
}

void window::appInfo() {
    static bool on = true;

    ImGui::Begin("Window Info", &on);
        ImGui::Text("Clock Rate: %3f", deltaTime);
        ImGui::Text("Height: %3u", height);
        ImGui::Text("Width: %3u", width);

    ImGui::End();
}

void world::appInfo() {
    static bool on = true;

    ImGui::Begin("World Info", &on);

    static bool* worldItems = new bool[tableSize];


    for(int i = 0; i < tableSize; i++) {
        ImGui::Checkbox(objectNames[i].c_str(), &worldItems[i]);
    }

    ImGui::End();

}
