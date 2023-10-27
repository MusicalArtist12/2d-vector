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
void spawnPolyApp();


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
    World.objectTable.add("circle 2", physObject(circle)).pos = glm::vec3(0.0, 10.0, 0.0);

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

        spawnPolyApp();
        
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
        float framerate = 1.0f / Window.deltaTime; // this really needs to be an average of like 5...
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

    glm::vec3& external = myObject.forceVectors.add("External Force", glm::vec3(0.0f));

    ImGui::Begin(myObject.getID().c_str(), run);

    ImGui::Text("Acceleration: (%f, %f)", myObject.accel().x, myObject.accel().y);
    ImGui::Text("Force Vector: (%f, %f)", myObject.forceSum().x, myObject.forceSum().y);
    ImGui::Text("Radius: %f", myObject.getRadius());
    ImGui::Checkbox("Freeze", &myObject.isStatic);

    ImGui::InputFloat2("External Force", (float *)&external);
    ImGui::InputFloat2("Position", (float*)&myObject.pos);
    ImGui::InputFloat2("Velocity", (float*)&myObject.vel);
    

    ImGui::Text("Force Table");

    if (ImGui::BeginTable(myObject.getID().c_str(), 3)) {
        
        for(int i = 0; i < myObject.forceVectors.size(); i++) {
            
            ImGui::TableNextRow();
            
            ImGui::TableNextColumn();
            ImGui::Text("%s", myObject.forceVectors.getID(i).c_str());
            ImGui::TableNextColumn();
            ImGui::Text("%f", myObject.forceVectors.getRef(i).x);
            ImGui::TableNextColumn();
            ImGui::Text("%f", myObject.forceVectors.getRef(i).y);
        }
        ImGui::EndTable();
    }

    ImGui::End();
}

void spawnPolyApp() {
    bool* run = appBools.add("Spawn Polygon", new bool(true));

    if(!*run) return;

    ImGui::Begin("Spawn Polygon", run);

    static char ID[100] = "Hello World";
    static int numSides = 3;
    ImGui::InputText("Item ID", ID, IM_ARRAYSIZE(ID));
    ImGui::SliderInt("Number of Sides", &numSides, 3, 100);
    bool createObject = false;
    
    if(World.objectTable.hasEntry(ID)) {
        ImGui::Text("Cannot duplicate names");
    
    } else {
        createObject = ImGui::Button("Create");
    }
    
    if(createObject) {
        mesh* myMesh = new mesh(genPolygon(numSides));
        World.objectTable.add(std::string(ID), physObject(myMesh));
    }

    ImGui::End();
}