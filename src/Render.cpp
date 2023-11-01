#include "Render.h"

void render::updateCamera(camera& cam) {
    activeShader->bind();
    activeShader->setView(cam.view(), cam.projection());
}

void render::drawMesh(mesh* Mesh, glm::mat4 model) {
    activeShader->bind();

    if(!Mesh->generated) {
        generateBuffer(Mesh);
    }
    
    if(Mesh->mode == TRIANGLE) activeShader->drawMesh(Mesh->VAO, Mesh->index.size(), model);
}

void render::generateBuffer(mesh* Mesh) {
    activeShader->bind();
    activeShader->generateBuffer(&Mesh->VAO, &Mesh->VBO, &Mesh->EBO, Mesh->vertices.data(), Mesh->vertices.size(), Mesh->index.data(), Mesh->index.size());
    Mesh->generated = true;
}

 