#ifndef MESH_H
#define MESH_H

#include "Vertex.h"

#include <glad/gl.h>     
#include <GLFW/glfw3.h>

#include <vector>

enum IndexMode {
    TRIANGLE
};

struct mesh {
    std::vector <vertex> vertices;
    std::vector <unsigned int> index;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    bool generated;

    IndexMode mode = TRIANGLE;

    float radius = 0; // set by the constructor and determines the circle that entraps the object

    mesh(std::vector<vertex>& v, std::vector<unsigned int>& i);
};

#endif
