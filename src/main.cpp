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

Window window("Title");
RenderQueue renderQueue;
World world;
Camera mainCam;

Dictionary<bool *> appBools(26);

ImGuiWindowFlags myFlags;

void cameraApp(Camera& myCam, bool* run, std::string ID);
void windowApp(bool* run, std::string ID);
void worldApp(bool* run, std::string ID);
void objectSubApp(PhysObject& myObject);
void spawnPolyApp(bool* run, std::string ID);

const float PADDLE_SPEED = 5;

void cancelMovement(PhysObject& object, KeyCallback* self) {
    object.vel = glm::vec3(0, 0, 0);

    object.removeKeyCallback(self->name);
}

void moveUp(PhysObject& object, KeyCallback* self) {
    object.vel = glm::vec3(0, PADDLE_SPEED, 0);
    
    object.addKeyCallback("cancel up", self->key, GLFW_RELEASE, cancelMovement);
}

void moveDown(PhysObject& object, KeyCallback* self) {
    object.vel = glm::vec3(0, -PADDLE_SPEED, 0);
    
    object.addKeyCallback("cancel down", self->key, GLFW_RELEASE, cancelMovement);
}

void preCollisionCallback(PhysObject* self, PhysObject& objB, glm::vec3 collisionPoint, float deltaTime) {
    // self->vel = glm::vec3(0, 0, 0);
}

int main() { 
    renderQueue.activeShader = new Shader("data/shaders/gen.vert", "data/shaders/gen.frag");
    
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.Fonts->AddFontFromFileTTF("data/fonts/SourceCodePro-Regular.otf", 18);

    std::vector<Vertex> wallVertices = {
        Vertex(1, -1, 1, 1, 1, 1), 
        Vertex(1, 1, 1, 1, 1, 1), 
        Vertex(-1, -1, 1, 1, 1, 1), 
        Vertex(-1, 1, 1, 1, 1, 1), 
    };

    std::vector<unsigned int> wallIndices = {
        0, 1, 2,
        1, 2, 3
    };

    world.add(PhysObject(genPolygon(32), "pong ball"));
    
    world.add(PhysObject(Mesh(wallVertices, wallIndices), "left paddle")).pos = glm::vec3(-10, 0, 0);
    world.add(PhysObject(Mesh(wallVertices, wallIndices), "right paddle")).pos = glm::vec3(10, 0, 0);

    world.add(PhysObject(Mesh(wallVertices, wallIndices), "top wall")).pos = glm::vec3(0, 10, 0);
    world.add(PhysObject(Mesh(wallVertices, wallIndices), "bottom wall")).pos = glm::vec3(0, -10, 0);

    world.entry("left paddle").isStatic = true;
    world.entry("right paddle").isStatic = true;
    world.entry("left paddle").scale = glm::vec3(0.25, 1, 1);
    world.entry("right paddle").scale = glm::vec3(0.25, 1, 1);
    world.entry("pong ball").scale = glm::vec3(0.5);
    world.entry("pong ball").mass = 1.0;
    world.entry("left paddle").mass = 0.1;
    world.entry("right paddle").mass = 0.1;


    world.entry("top wall").scale = glm::vec3(10.0, 0.1, 1);
    world.entry("bottom wall").scale = glm::vec3(10.0, 0.1, 1);
    world.entry("top wall").isStatic = true;
    world.entry("bottom wall").isStatic = true;

    world.entry("left paddle").addKeyCallback(
        "up",
        GLFW_KEY_W,
        GLFW_PRESS,
        moveUp
    );

    world.entry("right paddle").addKeyCallback(
        "up",
        GLFW_KEY_I,
        GLFW_PRESS,
        moveUp
    );

    world.entry("left paddle").addKeyCallback(
        "down",
        GLFW_KEY_S,
        GLFW_PRESS,
        moveDown
    );

    world.entry("right paddle").addKeyCallback(
        "down",
        GLFW_KEY_K,
        GLFW_PRESS,
        moveDown
    );

    world.entry("left paddle").preCollisionCallback = preCollisionCallback;
    world.entry("right paddle").preCollisionCallback = preCollisionCallback;

    while (!window.shouldClose()) {
        window.refresh();
        
        mainCam.inputLoop(window);
        world.update(window, renderQueue);

        renderQueue.draw(mainCam, window.width, window.height);

        window.render();
        window.startImgui();

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Window")) {
                for (int i = 0; i < appBools.size(); i++) {
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

        if (*appBools.add("Demo Window", new bool(true))) { 
            ImGui::ShowDemoWindow(); 
        }
        

        window.renderImgui();
    }

    window.terminate();
    return 0;
}

void cameraApp(Camera& myCam, bool* run, std::string ID) {
    if (!*run) {
        return; 
    }

    if (!ImGui::Begin(ID.c_str(), run, myFlags)) {
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
    if (!*run) {
        return;
    }

    if (!ImGui::Begin(ID.c_str(), run, myFlags)) {
        ImGui::End();
        return;
    }

    ImGui::Text("Clock Rate: %3f", window.deltaTime);
    float framerate = 1.0f / window.deltaTime; // this really needs to be an average of like 5...
    ImGui::Text("Framerate: %3f", framerate);
    ImGui::Text("Height: %3u", window.height);
    ImGui::Text("Width: %3u", window.width);

    ImGui::ColorPicker3("background", (float*)&window.clearColor);

    ImGui::Text("Draw Wireframe: ");
    ImGui::SameLine();
    if (renderQueue.drawWireframe == false and ImGui::Button("Enable")) {
        renderQueue.drawWireframe = true;
    }
    if (renderQueue.drawWireframe == true and ImGui::Button("Disable")) {
        renderQueue.drawWireframe = false;
    }

    ImGui::End();
}

void worldApp(bool* run, std::string ID) {
    if (!*run) {
        return;
    }

    if (!ImGui::Begin(ID.c_str(), run, myFlags)) {
        ImGui::End();
        return;
    }

    ImGui::Text("Physics Settings");
    ImGui::DragFloat("Gravity", &world.grav);
    ImGui::InputInt("Counts Per Frame", &world.countsPerFrame, 1);
    ImGui::InputInt("TimeScalePercent", &world.timeScalePercent, 1);

    ImGui::Checkbox("Use Gravity", &world.useGravity);
    ImGui::Checkbox("Use Collisions", &world.useCollisions);
    ImGui::Checkbox("Paused", &world.isPaused);
    ImGui::Text("Number of Objects: %i", world.tableSize());

    for (int i = 0; i < world.tableSize(); i++) {
        if (ImGui::CollapsingHeader( world.getRef(i).ID.c_str() ) ) {
            objectSubApp(world.getRef(i));
        }
    }

    ImGui::End();
}

void objectSubApp(PhysObject& myObject) {
    ImGui::PushID(&myObject);
    
    ImGui::DragFloat2("Position", (float*)&myObject.pos);
    ImGui::DragFloat2("Velocity", (float*)&myObject.vel);
    ImGui::SameLine(); ImGui::Text("(%f)", glm::length(myObject.vel));
    ImGui::Text("Acceleration: (%f, %f) |%f|", myObject.accel().x, myObject.accel().y, glm::length(myObject.accel()));
    ImGui::Text("Momentum: (%f, %f) |%f|", myObject.momentum().x, myObject.momentum().y, glm::length(myObject.momentum()));
    ImGui::Text("Kinetic Energy: %f", myObject.kineticEnergy());
    ImGui::Checkbox("isStatic", &myObject.isStatic);
    ImGui::Checkbox("isGhost", &myObject.isGhost);
    ImGui::InputFloat("Mass", &myObject.mass, 0.1);


    if (ImGui::TreeNode("Mesh Data")) {
        ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x, 260), false, ImGuiWindowFlags_HorizontalScrollbar);

        BufferMap bf = BufferMap(&myObject.myMesh.VAO, &myObject.myMesh.VBO, GL_ARRAY_BUFFER, GL_READ_WRITE);
        Vertex* vertices = (Vertex*)bf.data;

        for (int i = 0; i < myObject.myMesh.numVertices; i++) {
            ImGui::PushID(&vertices[i]);
            ImGui::DragFloat2("Position", &vertices[i].pos[0]);
            ImGui::SameLine(); ImGui::ColorEdit3("Color", &vertices[i].rgba[0]);
            ImGui::PopID();
        }

        ImGui::EndChild();
        ImGui::TreePop();


    }

    ImGui::PopID();
}


void spawnPolyApp(bool* run, std::string ID) {
    if (!*run) {
        return;
    }

    if (!ImGui::Begin(ID.c_str(), run, myFlags)) {
        ImGui::End();
        return;
    }

    ImGui::End();
}