#include "Render.h"

void RenderQueue::drawMesh(DrawData& data) {
    activeShader->bind();
    if (drawWireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } 
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (!data.mesh->generated) {
        generateBuffer(data.mesh);
    }

    if (!data.mesh->upToDate) {
        updateBuffer(data.mesh);
    }
    
    
    if (data.mesh->mode == TRIANGLE) {
        activeShader->drawMesh(data.mesh->VAO, data.mesh->index.size(), data.model);
    }
}

void RenderQueue::generateBuffer(Mesh* mesh) {
    activeShader->bind();

    activeShader->generateBuffer(&mesh->VAO, &mesh->VBO, &mesh->EBO, mesh->vertices.data(), mesh->vertices.size(), mesh->index.data(), mesh->index.size());
    mesh->generated = true;
    mesh->upToDate = true;
}

void RenderQueue::updateBuffer(Mesh* mesh) {
    activeShader->bind();
    activeShader->updateBuffer(&mesh->VAO, &mesh->VBO, &mesh->EBO, mesh->vertices.data(), mesh->vertices.size(), mesh->index.data(), mesh->index.size());
    mesh->upToDate = true;
}

void RenderQueue::draw(Camera& cam, int width, int height) {
    activeShader->bind();
    activeShader->setView(cam.view(), cam.projection(width, height));

    for (int i = 0; i < queue.size(); i++) {
        drawMesh(queue.back());
        queue.pop_back();
    }

    activeShader->unbind();
}