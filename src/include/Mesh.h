#ifndef MESH_H
#define MESH_H

#include <glad/gl.h>     
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

enum IndexMode {
    TRIANGLE
};

struct vertex {
    float pos[3];
    float rgba[4];

    vertex() {}

    vertex(float x, float y, float r, float g, float b, float a) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = 0.0;

        rgba[0] = r;
        rgba[1] = g;
        rgba[2] = b;
        rgba[3] = a;
    }
};

struct mesh {
    std::vector <vertex> vertices;
    std::vector <unsigned int> index;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    bool generated;

    IndexMode mode = TRIANGLE;

    mesh(std::vector<vertex>& v, std::vector<unsigned int>& i);
    float radius();
    glm::vec3 center();
};

void setColor(mesh* myMesh, float r, float g, float b, float a);

mesh genPolygon(int numSides);

#endif
