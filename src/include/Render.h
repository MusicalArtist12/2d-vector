#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>

struct drawData {
    mesh* myMesh;
    glm::mat4 model;
    
    drawData(mesh* m, glm::mat4 mod): myMesh(m), model(mod) {}
};

class renderQueue {
    private:
        void drawMesh(drawData& data);

    public:
        shader* activeShader;
        std::vector<drawData> queue;
        
        bool drawWireframe = false;
        void generateBuffer(mesh* Mesh);

        void draw(camera& cam, int width, int height);
};

#endif
