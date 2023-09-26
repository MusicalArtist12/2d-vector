#pragma once

#include <glad/gl.h>     
#include <GLFW/glfw3.h>  

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <imgui.h>

#include "WindowEngine.cpp"
#include "Objects/2DObject.cpp"
#include "RenderEngine.cpp"
#include "dictionary.cpp"


namespace physics {

    dictionary<physObject> worldObjects;

    bool Physics = false;
    float grav = 0.0f;

    std::string* objects;

    void calculatePhysics(physObject& currentObj) {
        float* pos[] = { &currentObj.objMatrix[3][0], &currentObj.objMatrix[3][1] };
        float* vel[] = { &currentObj.velocity.x, &currentObj.velocity.y };
        float* accel[] = { &currentObj.accel.x, &currentObj.accel.y };
        
        float tmp_accel = currentObj.accel.y;

        if(*pos[1] >= 0) {
            currentObj.accel.y += grav;
        }

        currentObj.velocity += currentObj.accel * window::deltaTime;

        if(*pos[1] <= 0) {
            *pos[1] = 0.0f;

            if(*vel[1] < 0) *vel[1] = 0.0f;
        }

        currentObj.objMatrix = glm::translate(currentObj.objMatrix, currentObj.velocity * window::deltaTime);


        currentObj.accel.y = tmp_accel;
    }

    void render() {
        objects = worldObjects.getArrayOfItems();

        //window::deltaTime

        for(int i = 0; i < worldObjects.size(); i++){
            physObject& currentObj = worldObjects.getItem(objects[i]);

            if(Physics) {
                // floor
                calculatePhysics(currentObj);
            }


            currentObj.render();
        }
    }

    bool* worldItems;

    void appInfo() {
        static bool on = true;

        static bool generated = false;
        if(generated == false) {
            worldItems = new bool[worldObjects.size()];
            
            for(int i = 0; i < worldObjects.size(); i++ ) {
                worldItems[i] = false;
            }
            
            generated = true;
        }

        ImGui::Begin("Physics", &on);
        ImGui::Checkbox("Physics", &Physics);
        ImGui::SliderFloat("Gravity", &grav, -9.8, 9.8);

        ImGui::Text("World Items");

        for(int i = 0; i < worldObjects.size(); i++) {
            ImGui::Checkbox(objects[i].c_str(), &worldItems[i]);
        }

        ImGui::End();

        for(int i = 0; i < worldObjects.size(); i++) {
            
            
            if(worldItems[i] == true) {
                ImGui::Begin(objects[i].c_str(), &on);

                physObject& currentObj = worldObjects.getItem(objects[i]);

                float pos[] = {currentObj.objMatrix[3][0], currentObj.objMatrix[3][1]};
                float vel[] = {currentObj.velocity[0], currentObj.velocity[1]};
                float accel[] = {currentObj.accel[0], currentObj.accel[1]};

                ImGui::SliderFloat2("position", pos, -100, 100);
                ImGui::SliderFloat2("velocity", vel, -100, 100);
                ImGui::SliderFloat2("acceleration", accel, -100, 100);

                currentObj.objMatrix[3][0] = pos[0];
                currentObj.objMatrix[3][1] = pos[1];

                currentObj.velocity[0] = vel[0];
                currentObj.velocity[1] = vel[1];

                currentObj.accel[0] = accel[0];
                currentObj.accel[1] = accel[1];

                ImGui::End();
            }
        }


    }

    void addItem(physObject n) {
        worldObjects.addItem(n, n.id);
    }
}

