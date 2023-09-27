#include "gfxEngine/RenderEngine.h"

namespace render {
    shader* activeShader;
    camera* activeCamera;
}

void render::updateCamera() {
    activeShader->bind();
    activeShader->setView(activeCamera->view(), activeCamera->projection());
}

void render::drawMesh(mesh* Mesh, glm::mat4 model) {
    activeShader->bind();
    activeShader->drawMesh(Mesh->VAO, Mesh->index.size(), model);
}

void render::generateBuffer(mesh* Mesh) {
    activeShader->bind();
    activeShader->generateBuffer(&Mesh->VAO, &Mesh->VBO, &Mesh->EBO, Mesh->vertices.data(), Mesh->vertices.size(), Mesh->index.data(), Mesh->index.size());
    Mesh->generated = true;
}

 