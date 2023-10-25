#include <iostream>
#include <iomanip>
#include <string>

#include <glad/gl.h>     
#include <GLFW/glfw3.h>  

#include "Window.h"
#include "Render.h"
#include "World.h"

window Window("Title");
render Render;
world World;

dictionary<bool *> appBools(26);

void appApp();
void cameraApp(camera* myCam);
void windowApp();
void worldApp();
void objectApp(physObject& myObject);

std::vector<vertex> verts = {
    vertex(-1.0, -1.0, 1.0, 1.0, 1.0, 1.0),
    vertex(1.0, -1.0, 1.0, 1.0, 1.0, 1.0),
    vertex(1.0, 1.0, 1.0, 1.0, 1.0, 1.0),
    vertex(-1.0, 1.0, 1.0, 1.0, 1.0, 1.0)
};

std::vector<unsigned int> ind = {
    0, 1, 2, 
    0, 2, 3
};

mesh* square = new mesh(verts, ind);

int main() { 
    mesh* circle = new mesh(genPolygon(100));

    World.objectTable.add("circle", physObject(circle)).pos = glm::vec3(0.0, 5.0, 0.0);
    physObject& Ground = World.objectTable.add("ground", physObject(square));

    Ground.transformMatrix = glm::scale(glm::vec3(1000, 100.0, 1.0));
    Ground.isStatic = true;
    Ground.pos = glm::vec3(0.0f, -100.0f, 0.0f);
    setColor(square, 0.0f, 0.0f, 0.0f, 1.0f);


    Window.loadFont("data/fonts/SourceCodePro-Regular.otf", 36);

    Render.activeShader = new shader("data/shaders/gen.vert", "data/shaders/gen.frag");
    Render.activeCamera = new camera;
    
    glm::vec3 background_color((102.0f/255.0f), (180.0f/255.0f), (222.0f/255.0f));


    while(!Window.shouldClose()) {
        Window.refresh();
        ImGui::NewFrame();

        glClearColor(background_color[0], background_color[1], background_color[2], 1.0f);  
        
        Render.updateCamera();
        Render.activeCamera->InputLoop(Window.deltaTime);
        World.update();

        appApp();
        cameraApp(Render.activeCamera);
        windowApp();
        worldApp();

        for(int i = 0; i < World.objectTable.size(); i++) {
            objectApp(World.objectTable.getRef(i));
        }
        
        Window.render(); 
    }

    Window.terminate();
    return 0;
}

void appApp() {
    ImGui::Begin("Widgets");

    for(int i = 0; i < appBools.size(); i++) {

        bool* myBool = appBools.getRef(i);

        ImGui::Checkbox(appBools.getID(i).c_str(), myBool);
    }

    ImGui::End();
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
        ImGui::Text("Clock Rate: %3f", Window.deltaTime);
        float framerate = 1.0f / Window.deltaTime;
        ImGui::Text("Framerate: %3f", framerate);
        ImGui::Text("Height: %3u", Window.height);
        ImGui::Text("Width: %3u", Window.width);

    ImGui::End();
}

void worldApp() {
    bool* run = appBools.add("World App", new bool(true));

    if(!*run) return;

    ImGui::Begin("World Info", run);

    ImGui::Text("Physics Settings");
    ImGui::InputFloat("Gravity", &World.grav);
    ImGui::Checkbox("Use Physics", &World.usePhysics);
    ImGui::Text("");
    ImGui::Text("Number of Objects: %i", World.objectTable.size());

    for(int i = 0; i < World.objectTable.size(); i++) {
        std::string appName = "Object: " + World.objectTable.getID(i);

        bool* myBool = appBools.add(appName, new bool(false));

        ImGui::Checkbox(World.objectTable.getID(i).c_str(), myBool);
    }

    ImGui::End();

}


void objectApp(physObject& myObject) {
    std::string appName = "Object: " + myObject.getID();

    bool* run = appBools.add(appName, new bool(true));

    if(!*run) return;

    ImGui::Begin(myObject.getID().c_str(), run);

    ImGui::Text("Position: (%f, %f)", myObject.pos.x, myObject.pos.y);
    ImGui::Text("Velocity: (%f, %f)", myObject.vel.x, myObject.vel.y);
    ImGui::Text("Acceleration: (%f, %f)", myObject.accel.x, myObject.accel.y);
    ImGui::Text("Distance from Origin: %f", myObject.distanceFrom(glm::vec3(0.0)));
    ImGui::Text("Force Vector: (%f, %f)", myObject.forceSum.x, myObject.forceSum.y);

    ImGui::End();
}
