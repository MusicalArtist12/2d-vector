#include <iostream>
#include <iomanip>
#include <string>

#include <glad/gl.h>     
#include <GLFW/glfw3.h>  

#include "glCore/WindowEngine.h" // Layer between OS and code
#include "gfxEngine/RenderEngine.h"
#include "physicsEngine/PhysicsEngine.h"
#include "gfxEngine/WorldEngine.h"


void loop();

dictionary<bool *> appBools(26);

void cameraApp(camera* myCam);
void windowApp();
void worldApp();
void objectApp(physObject& myObject, std::string id);

int main() { 
    mesh* triangle = new mesh(genPolygon(3));
    mesh* square = new mesh(genPolygon(4));
    mesh* pentagon = new mesh(genPolygon(5));
    mesh* hexagon = new mesh(genPolygon(6));
    mesh* circle = new mesh(genPolygon(100));

    world::addItem(physObject(triangle), "triangle");
    world::addItem(physObject(square), "square");
    world::addItem(physObject(pentagon), "pentagon");
    world::addItem(physObject(hexagon), "hexagon");
    world::addItem(physObject(circle), "circle");

    world::objectTable.pullItem("circle");

    window::init();
    window::loadFont("data/fonts/SourceCodePro-Regular.otf", 16);

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
        
        cameraApp(render::activeCamera);
        windowApp();
        worldApp();

        world::update();

        window::render();  
    }
}

void cameraApp(camera* myCam) {
    if(!appBools.hasItem("Camera App")) {
        appBools.addItem(new bool(true), "Camera App");
    }

    bool* run = appBools.getItem("Camera App");
    if(!*run) return; 

    ImGui::Begin("Camera Info", run);

    ImGui::InputFloat2("Camera Position", (float *)&myCam->pos);
    ImGui::InputFloat("Camera Scale", &myCam->scale);
    ImGui::InputFloat("Camera Speed", &myCam->speed);

    ImGui::End();

}

void windowApp() {
    if(!appBools.hasItem("Window App")) {
        appBools.addItem(new bool(true), "Window App");
    }
    
    bool* run = appBools.getItem("Window App");
    if(!*run) return;

    ImGui::Begin("Window Info", run);
        ImGui::Text("Clock Rate: %3f", window::deltaTime);
        ImGui::Text("Height: %3u", window::height);
        ImGui::Text("Width: %3u", window::width);

    ImGui::End();
}

void worldApp() {
    if(!appBools.hasItem("World App")) {
        appBools.addItem(new bool(true), "World App");
    }

    bool* run = appBools.getItem("World App");

    if(!*run) return;

    ImGui::Begin("World Info", run);

    ImGui::InputFloat("Gravity", &physics::grav);
    ImGui::Checkbox("Use Physics", &world::usePhysics);

    for(int i = 0; i < world::tableSize; i++) {
        std::string appName = world::objectNames[i] + " App";

        if(!appBools.hasItem(appName.c_str())) {
            appBools.addItem(new bool(true), appName.c_str());
        }

        ImGui::Checkbox(world::objectNames[i].c_str(), appBools.getItem(appName.c_str()));
    }

    ImGui::End();

    for(int i = 0; i < world::tableSize; i++) {
        objectApp(world::objectTable.getItem(world::objectNames[i]), world::objectNames[i]);
    }
}

void objectApp(physObject& myObject, std::string id) {
    std::string appName = id + " App";

    if(!appBools.hasItem(appName)) {
        appBools.addItem(new bool(true), appName);
    }

    bool* run = appBools.getItem(appName);

    if(!*run) return;

    ImGui::Begin(id.c_str(), run);

    ImGui::InputFloat2("Position", (float *)&myObject.pos);
    ImGui::InputFloat2("Velocity", (float *)&myObject.vel);
    ImGui::InputFloat2("Acceleration", (float *)&myObject.accel);

    ImGui::End();
}