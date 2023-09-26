#include <iostream>
#include <iomanip>
#include <string>

#include <glad/gl.h>     
#include <GLFW/glfw3.h>  

#include "Utils.cpp"

#include "WindowEngine.cpp" // Layer between OS and code

#include "Objects/Shader.cpp"
#include "Objects/Camera.cpp"
#include "Objects/Mesh.cpp"

#include "Objects/PhysObject.cpp"

#include "RenderEngine.cpp"
#include "PhysicsEngine.cpp"

#include "WorldEngine.cpp"

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
    window::init();

    window::loadFont("assets/fonts/SourceCodePro-Regular.otf", 32);

    mesh mesh0(mesh0_vertices, mesh0_index);
    physObject myObject(&mesh0);

    world::addItem(myObject, "rainbow cube");

    render::activeShader = new shader("assets/shaders/gen.vert", "assets/shaders/gen.frag");
    render::activeCamera = new camera;

    loop();

    window::terminate();
    
    return 0;
}

glm::vec3 background_color(0.0f, 0.0f, 0.0f);

void loop() {
    
    while(!glfwWindowShouldClose(window::window)) {
        render::bind();

        window::refresh();
        ImGui::NewFrame();

        glClearColor(background_color[0], background_color[1], background_color[2], 1.0f);  
        
        render::updateCamera();
        render::activeCamera->InputLoop(window::deltaTime);
        render::activeCamera->getMousePositionRelative();

        world::update();
        
        render::activeCamera->appInfo();
        window::appInfo();

        window::render();
        
    }
}

