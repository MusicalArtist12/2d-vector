#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>

struct DrawData {
    Mesh* mesh;
    glm::mat4 model;
    
    DrawData(Mesh* m, glm::mat4 mod): mesh(m), model(mod) {}
};

class RenderQueue {
    private:
        void drawMesh(DrawData& data);

    public:
        Shader* activeShader;
        std::vector<DrawData> queue;
        
        bool drawWireframe = false;
        void generateBuffer(Mesh* mesh, std::vector<Vertex>& v, std::vector<unsigned int>& i);

        void draw(Camera& cam, int width, int height);
};

#endif
