#ifndef MESH_H
#define MESH_H

#include <glad/gl.h>     
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


#include <vector>

enum IndexMode {
    TRIANGLE
};

struct Vertex {
    glm::vec3 pos;
    glm::vec4 rgba;

    Vertex() {}

    Vertex(float x, float y, float r, float g, float b, float a) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = 0.0;

        rgba[0] = r;
        rgba[1] = g;
        rgba[2] = b;
        rgba[3] = a;
    }
};

struct Mesh {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    const int numVertices;
    const int numIndices;

    IndexMode mode = TRIANGLE;

    Mesh(std::vector<Vertex>& v, std::vector<unsigned int>& i);
    float radius();
    glm::vec3 center(Vertex* vertices);
};

void setColor(Mesh* myMesh, float r, float g, float b, float a);
Mesh genPolygon(int numSides);

#endif
