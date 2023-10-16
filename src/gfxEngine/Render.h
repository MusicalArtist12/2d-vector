#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include "glCore/Shader.h"
#include "gfxEngine/Camera.h"
#include "glCore/Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class render {
    public:
        shader* activeShader;
        camera* activeCamera;

        void updateCamera();
        void drawMesh(mesh* Mesh, glm::mat4 model);
        void generateBuffer(mesh* Mesh);

};

#endif
