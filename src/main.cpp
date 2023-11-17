#include <iostream>
#include <iomanip>
#include <string>

#include <glad/gl.h>     
#include <GLFW/glfw3.h>  
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Window.h"
#include "Render.h"
#include "World.h"

window Window("Title");
render Render;
world World;
camera mainCam;

dictionary<bool *> appBools(26);

ImGuiWindowFlags myFlags;

void cameraApp(camera& myCam, bool* run, std::string ID);
void windowApp(bool* run, std::string ID);
void worldApp(bool* run, std::string ID);
void objectSubApp(physObject& myObject);
void spawnPolyApp(bool* run, std::string ID);


int main() { 
    Render.activeShader = new shader("data/shaders/gen.vert", "data/shaders/gen.frag");
    
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.Fonts->AddFontFromFileTTF("data/fonts/SourceCodePro-Regular.otf", 18);

    World.add("mine", physObject(new mesh(genPolygon(8))));

    
    while(!Window.shouldClose()) {
        Window.refresh();
        
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Window")) {
                for(int i = 0; i < appBools.size(); i++) {
                    if(ImGui::MenuItem(appBools.getID(i).c_str(), "", appBools.getRef(i))) {}
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

        cameraApp(mainCam, appBools.add("Camera App", new bool(true)), "Camera App");
        windowApp(appBools.add("Window Info", new bool(true)), "Window Info");
        worldApp(appBools.add("World", new bool(true)), "World");
        spawnPolyApp(appBools.add("Spawn Polygon", new bool(true)), "Spawn Polygon");

        if(*appBools.add("Demo Window", new bool(true))) { ImGui::ShowDemoWindow(); }
        
        World.update(Window.deltaTime);
        Render.updateCamera(mainCam, Window.width, Window.height);
        mainCam.InputLoop(Window);
        
        Render.draw();
        Window.render(); 
    }

    Window.terminate();
    return 0;
}

void cameraApp(camera& myCam, bool* run, std::string ID) {
    if(!*run) return; 

    if(!ImGui::Begin(ID.c_str(), run, myFlags)) {
        ImGui::End();
        return;
    }

    ImGui::DragFloat2("Camera Position", (float *)&myCam.pos);
    ImGui::DragFloat("Camera Scale", &myCam.scale,1.0f, 0.0f, FLT_MAX);
    ImGui::DragFloat("Camera Speed", &myCam.speed,1.0f, 0.0f, FLT_MAX);

    ImGui::End();

    return;
}

void windowApp(bool* run, std::string ID) {
    if(!*run) return;

    if(!ImGui::Begin(ID.c_str(), run, myFlags)) {
        ImGui::End();
        return;
    }

        ImGui::Text("Clock Rate: %3f", Window.deltaTime);
        float framerate = 1.0f / Window.deltaTime; // this really needs to be an average of like 5...
        ImGui::Text("Framerate: %3f", framerate);
        ImGui::Text("Height: %3u", Window.height);
        ImGui::Text("Width: %3u", Window.width);

        
        ImGui::ColorPicker3("background", (float*)&Window.clearColor);

        ImGui::Text("Draw Wireframe: ");
        ImGui::SameLine();
        if(Render.drawWireframe == false and ImGui::Button("Enable")) Render.drawWireframe = true;
        if(Render.drawWireframe == true and ImGui::Button("Disable")) Render.drawWireframe = false;

    ImGui::End();
}

void worldApp(bool* run, std::string ID) {
    if(!*run) return;

    if(!ImGui::Begin(ID.c_str(), run, myFlags)) {
        ImGui::End();
        return;
    }

    ImGui::Text("Physics Settings");
    ImGui::DragFloat("Gravity", &World.grav);
    ImGui::InputInt("Counts Per Frame", &World.countsPerFrame, 1);
    ImGui::Checkbox("Use Physics", &World.usePhysics);
    ImGui::Text("");
    ImGui::Text("Number of Objects: %i", World.tableSize());

    for (int i = 0; i < World.tableSize(); i++) {
        if (ImGui::CollapsingHeader( World.getID(i).c_str()) ) {
            objectSubApp(World.getRef(i));
        }
    }

    ImGui::End();
}


void objectSubApp(physObject& myObject) {
    ImGui::PushID(&myObject);
    
    ImGui::DragFloat2("Position", (float*)&myObject.pos);
    
    ImGui::DragFloat2("Velocity", (float*)&myObject.vel);
    ImGui::SameLine(); ImGui::Text("(%f)", glm::length(myObject.vel));
    ImGui::Text("Acceleration: (%f, %f) |%f|", myObject.accel().x, myObject.accel().y, glm::length(myObject.accel()));
    ImGui::Text("Momentum: (%f, %f) |%f|", myObject.momentum().x, myObject.momentum().y, glm::length(myObject.momentum()));
    ImGui::Text("Kinetic Energy: %f", myObject.kineticEnergy());

    if(ImGui::TreeNode("Mesh Data")) {
        ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x, 260), false, ImGuiWindowFlags_HorizontalScrollbar);

        for(int i = 0; i < myObject.myMesh->vertices.size(); i++) {
            ImGui::PushID(&myObject.myMesh->vertices[i]);
            ImGui::DragFloat2("Position", myObject.myMesh->vertices[i].pos);
            ImGui::SameLine(); ImGui::ColorEdit3("Color", myObject.myMesh->vertices[i].rgba);
            ImGui::PopID();
        }

        ImGui::EndChild();
        ImGui::TreePop();

        if(ImGui::Button("Update Mesh")) myObject.myMesh->generated=false;
    }

    ImGui::PopID();
}

void spawnPolyApp(bool* run, std::string ID) {
    if(!*run) return;

    if(!ImGui::Begin(ID.c_str(), run, myFlags)) {
        ImGui::End();
        return;
    }

    ImGui::End();
}