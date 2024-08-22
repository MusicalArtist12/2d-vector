#include "Render.h"

void RenderQueue::drawMesh(DrawData& data) {
    activeShader->bind();
    if (drawWireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } 
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    
    if (data.mesh->mode == TRIANGLE) {
        activeShader->drawMesh(data.mesh->VAO, data.mesh->numIndices, data.model);
    }
}

void RenderQueue::generateBuffer(Mesh* mesh, std::vector<Vertex>& v, std::vector<unsigned int>& i) {
    activeShader->generateBuffer(&mesh->VAO, &mesh->VBO, &mesh->EBO, v, i);
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