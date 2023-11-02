#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include "Camera.h"

#include "Shader.h"
#include "Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class render {
    public:
        shader* activeShader;
        
        bool drawWireframe = false;
        void updateCamera(camera& cam, int width, int height);
        void drawMesh(mesh* Mesh, glm::mat4 model);
        void generateBuffer(mesh* Mesh);
};

#endif
