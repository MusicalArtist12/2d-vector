#pragma once

#include "Mesh.cpp"

#include "../RenderEngine.cpp"

#include "../WindowEngine.cpp"
#include <imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <string>
#include <vector>

struct vec2ptr {
    float* pos[2];

    vec2ptr(float ref[2]) {
        pos[0] = &ref[0];
        pos[1] = &ref[1];
    }
    
};

class physObject {
    public: 
        mesh* objMesh;
        std::string id;

        glm::mat4 objMatrix = glm::mat4(1.0f);

        glm::vec3 accel = glm::vec3(0.0f);
        glm::vec3 velocity = glm::vec3(0.0f);

        std::vector<vec2ptr> vertices; // used for physics simulation

        void render();

        physObject(mesh* shape, std::string id);

        physObject(physObject& obj) {
            objMesh = obj.objMesh;
            id = obj.id;
        }

        physObject() {
            objMesh = nullptr;
            id = "";
        }


};

void physObject::render() {
    render::drawMesh(*objMesh, objMatrix);
}

physObject::physObject(mesh* shape, std::string i) {
    objMesh = shape;
    id = i;

    for(int i = 0; i < shape->vertices.size(); i++) {
        vertices.push_back(vec2ptr(shape->vertices[i].pos));
    }

    // get collection of vertex points, for future cpu-powered physics
}