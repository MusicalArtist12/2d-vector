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
camera mainCam;

dictionary<bool *> appBools(26);

int main() { 
    Window.loadFont("data/fonts/SourceCodePro-Regular.otf", 18);

    Render.activeShader = new shader("data/shaders/gen.vert", "data/shaders/gen.frag");
    
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    while(!Window.shouldClose()) {
        Window.refresh();

        World.update(Window.deltaTime);
        Render.updateCamera(mainCam, Window.width, Window.height);
        mainCam.InputLoop(Window);

        appApp();
        cameraApp(mainCam);
        windowApp();
        worldApp();
        spawnPolyApp();
        
        Window.render(); 
    }

    Window.terminate();
    return 0;
}