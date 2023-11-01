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
void objectSubApp(physObject& myObject);
void spawnPolyApp();


int main() { 
    Window.loadFont("data/fonts/SourceCodePro-Regular.otf", 18);

    Render.activeShader = new shader("data/shaders/gen.vert", "data/shaders/gen.frag");
    Render.activeCamera = new camera;
    
    glm::vec3 background_color((102.0f/255.0f), (180.0f/255.0f), (222.0f/255.0f));
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    while(!Window.shouldClose()) {
        Window.refresh();
        ImGui::NewFrame();


        glClearColor(background_color[0], background_color[1], background_color[2], 1.0f);  
        
        Render.updateCamera();
        Render.activeCamera->InputLoop(Window.deltaTime);

        World.update(Window.deltaTime);

        appApp();
        cameraApp(Render.activeCamera);
        windowApp();
        worldApp();
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
    ImGui::InputInt("Counts Per Frame", &World.countsPerFrame, 1);
    ImGui::Checkbox("Use Physics", &World.usePhysics);
    ImGui::Text("");
    ImGui::Text("Number of Objects: %i", World.tableSize());

    if (ImGui::TreeNode("Objects")) {
        for (int i = 0; i < World.tableSize(); i++) {
            // Use SetNextItemOpen() so set the default state of a node to be open. We could
            // also use TreeNodeEx() with the ImGuiTreeNodeFlags_DefaultOpen flag to achieve the same thing!
            if (i == 0) ImGui::SetNextItemOpen(true, ImGuiCond_Once);

            if (ImGui::TreeNode( (void*)(intptr_t)i, "%s", World.getID(i).c_str()) ) {
                objectSubApp(World.getRef(i));
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }

    ImGui::End();
}


void objectSubApp(physObject& myObject) {

    glm::vec3& external = myObject.forceVectors.add("External Force", glm::vec3(0.0f));

    ImGui::PushID(&myObject);
    
    static int clicked = 0;

    if(myObject.isStatic && ImGui::Button("Unfreeze")) {
        clicked = (clicked + 1) % 2;
        myObject.isStatic = clicked;
    } 

    else if(!myObject.isStatic && ImGui::Button("freeze")) {
        clicked = (clicked + 1) % 2;
        myObject.isStatic = clicked;
    }


    ImGui::SameLine();
    
    bool deleteMe = ImGui::Button("Delete");

    if (ImGui::BeginTable((myObject.getID() + "Info").c_str(), 3)) {
        ImGui::PushID(&external);

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("External Force");
        ImGui::TableNextColumn();
        ImGui::InputFloat2("", (float *)&external);
        ImGui::TableNextColumn();
        ImGui::Text(" (%f)", glm::length(external));

        ImGui::PopID();

        ImGui::PushID(&myObject.pos);

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Position");
        ImGui::TableNextColumn();
        ImGui::InputFloat2("", (float*)&myObject.pos);
        ImGui::TableNextColumn();
        ImGui::Text(" (%f)", glm::length(myObject.pos));

        ImGui::PopID();
        ImGui::PushID(&myObject.vel);

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Velocity");
        ImGui::TableNextColumn();
        ImGui::InputFloat2("", (float*)&myObject.vel);
        ImGui::TableNextColumn();
        ImGui::Text(" (%f)", glm::length(myObject.vel));
       
        ImGui::PopID();
        ImGui::EndTable();
    }

    ImGui::SliderFloat("Mass", &myObject.mass, 0.01, 100.0);
    
    ImGui::Text("Radius: %f", myObject.getRadius());
    ImGui::Text("Acceleration: (%f, %f) (%f)", myObject.accel().x, myObject.accel().y, glm::length(myObject.accel()));
    ImGui::Text("Momentum: (%f, %f) (%f)", myObject.momentum().x, myObject.momentum().y, glm::length(myObject.momentum()));
    ImGui::Text("Net Force Vector: (%f, %f) (%f)", myObject.forceSum().x, myObject.forceSum().y, glm::length(myObject.forceSum()));

    ImGui::PopID();

    if (ImGui::TreeNode("Force Table")) {

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

        ImGui::TreePop();
    }

    if(deleteMe) {
        World.remove(myObject.getID());
    }
}

void spawnPolyApp() {
    bool* run = appBools.add("Spawn Polygon", new bool(true));

    if(!*run) return;

    ImGui::Begin("Spawn Polygon", run);

    static char ID[100] = "";
    static int numSides = 3;
    ImGui::InputText("Item ID", ID, IM_ARRAYSIZE(ID));
    ImGui::SliderInt("Number of Sides", &numSides, 3, 100);
    bool createObject = false;
    
    if(World.hasEntry(ID)) {
        ImGui::Text("Cannot duplicate names");
    }
    
    else if(std::string(ID).length() == 0) {
        ImGui::Text("Name cannot be empty");
    }

     else {
        createObject = ImGui::Button("Create");
    }
    
    if(createObject) {
        mesh* myMesh = new mesh(genPolygon(numSides));
        World.add(std::string(ID), physObject(myMesh));
        
    }

    ImGui::End();
}