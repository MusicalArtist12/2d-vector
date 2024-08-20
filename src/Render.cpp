#include "Render.h"

void renderQueue::drawMesh(drawData& data) {
    activeShader->bind();
    if (drawWireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } 
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (!data.myMesh->generated) {
        generateBuffer(data.myMesh);
    }
    
    if (data.myMesh->mode == TRIANGLE) {
        activeShader->drawMesh(data.myMesh->VAO, data.myMesh->index.size(), data.model);
    }
}

void renderQueue::generateBuffer(mesh* Mesh) {
    activeShader->bind();
    activeShader->generateBuffer(&Mesh->VAO, &Mesh->VBO, &Mesh->EBO, Mesh->vertices.data(), Mesh->vertices.size(), Mesh->index.data(), Mesh->index.size());
    Mesh->generated = true;
}

void renderQueue::draw(camera& cam, int width, int height) {
    activeShader->bind();
    activeShader->setView(cam.view(), cam.projection(width, height));

    for (int i = 0; i < queue.size(); i++) {
        drawMesh(queue.back());
        queue.pop_back();
    }
}