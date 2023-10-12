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

    world::objectTable.add("triangle", physObject(triangle)).pos = glm::vec3(0.0, 5.0, 0.0);

    window::init();
    window::loadFont("data/fonts/SourceCodePro-Regular.otf", 36);

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
    bool* run = appBools.add("Camera App", new bool(true));
    if(!*run) return; 

    ImGui::Begin("Camera Info", run);

    ImGui::InputFloat2("Camera Position", (float *)&myCam->pos);
    ImGui::InputFloat("Camera Scale", &myCam->scale);
    ImGui::InputFloat("Camera Speed", &myCam->speed);

    ImGui::End();

}

void windowApp() {
    bool* run = appBools.add("Window App", new bool(true));
    if(!*run) return;

    ImGui::Begin("Window Info", run);
        ImGui::Text("Clock Rate: %3f", window::deltaTime);
        ImGui::Text("Height: %3u", window::height);
        ImGui::Text("Width: %3u", window::width);

    ImGui::End();
}

void worldApp() {
    bool* run = appBools.add("World App", new bool(true));

    if(!*run) return;

    ImGui::Begin("World Info", run);

    ImGui::InputFloat("Gravity", &physics::grav);
    ImGui::Checkbox("Use Physics", &world::usePhysics);

    for(int i = 0; i < world::tableSize; i++) {
        std::string appName = world::objectNames[i] + " App";

        bool* myBool = appBools.add(appName, new bool(false));

        ImGui::Checkbox(world::objectNames[i].c_str(), myBool);
    }

    ImGui::End();

    for(int i = 0; i < world::tableSize; i++) {
        objectApp(world::objectTable.entry(world::objectNames[i]), world::objectNames[i]);
    }
}

void objectApp(physObject& myObject, std::string id) {
    std::string appName = id + " App";

    bool* run = appBools.add(appName, new bool(true));

    if(!*run) return;

    ImGui::Begin(id.c_str(), run);

    ImGui::Text("Position: (%f, %f)", myObject.pos.x, myObject.pos.y);
    ImGui::Text("Velocity: (%f, %f)", myObject.vel.x, myObject.vel.y);
    ImGui::Text("Acceleration: (%f, %f)", myObject.accel.x, myObject.accel.y);

    ImGui::End();
}