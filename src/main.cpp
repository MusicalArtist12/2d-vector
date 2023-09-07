#include <iostream>
#include <iomanip>
#include <string>

#include <glad/gl.h>     
#include <GLFW/glfw3.h>  

#include "Utils.cpp"

// Physics Engine
#include "PhysicsEngine.cpp"


// Window Engine
#include "WindowEngine.cpp" // Layer between OS and code
#include "GuiEngine.cpp" // can come first due to function pointers

// Objects
#include "Objects/Camera.cpp"
#include "Objects/Mesh.cpp"

// Render Engine
#include "RenderEngine.cpp" // Translation layer between specific shader code




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

mesh* meshptr; // temp, located in global for loop()

void loop();

int main() {
    window::init();
    gui::init();
    gui::loadFont("bin/fonts/SourceCodePro-Regular.otf", 32);

    gui::loadFont("bin/fonts/SourceCodePro-Regular.otf", 32);

    mesh mesh0(mesh0_vertices, mesh0_index);
    meshptr = &mesh0;

    render::activeShader = new shader("bin/shaders/gen.vert", "bin/shaders/gen.frag");
    render::activeCamera = new camera;

    loop();

    gui::terminate();
    window::terminate();
    
    return 0;
}

glm::vec3 background_color(0.0f, 0.0f, 0.0f);

void loop() {
    
    while(!glfwWindowShouldClose(window::window)) {
        physics::updateClock();

        render::activeShader->bind();
        // check for changes
        window::refresh();
        gui::refresh();

        //ImGui::ShowStyleEditor();

        glClearColor(background_color[0], background_color[1], background_color[2], 1.0f);  
        render::updateCamera();
        render::activeCamera->InputLoop(physics::deltaTime);

        render::drawMesh(*meshptr, glm::mat4(1.0f));

        window::appInfo();
        gui::render();
    }
}

