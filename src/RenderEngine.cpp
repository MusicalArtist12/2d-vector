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

    void updateCamera();
    void drawMesh(mesh& Mesh, glm::mat4 model);
}

void render::updateCamera() {
    activeShader->bind();
    activeShader->setUniform4fv("view", activeCamera->view());
    activeShader->setUniform4fv("projection", activeCamera->projection());
}

void render::drawMesh(mesh& Mesh, glm::mat4 model) {
    activeShader->bind();
    activeShader->setUniform4fv("model", model);

    glBindVertexArray(Mesh.VAO);
    
    glDrawElements(GL_TRIANGLES, Mesh.index.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

 