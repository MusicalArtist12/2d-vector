#pragma once

#include "Objects/Shader.cpp"
#include "Objects/Camera.cpp"
#include "Objects/Mesh.cpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <string> 
#include <iostream>

namespace render {
    shader* activeShader;
    camera* activeCamera;

    inline void updateCamera();
    inline void drawMesh(mesh* Mesh, glm::mat4& model);
    inline void generateBuffer(mesh* Mesh);
    inline void bind();
}

inline void render::updateCamera() {
    activeShader->setView(activeCamera->view(), activeCamera->projection());
}

inline void render::drawMesh(mesh* Mesh, glm::mat4& model) {
    activeShader->drawMesh(Mesh->VAO, Mesh->index.size(), model);
}

inline void render::generateBuffer(mesh* Mesh) {
    activeShader->generateBuffer(&Mesh->VAO, &Mesh->VBO, &Mesh->EBO, Mesh->vertices.data(), Mesh->vertices.size(), Mesh->index.data(), Mesh->index.size());
    Mesh->generated = true;
}

inline void render::bind() {
    activeShader->bind();
}
 