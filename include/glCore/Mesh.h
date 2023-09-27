#ifndef MESH_H
#define MESH_H

#include "Vertex.h"

#include <glad/gl.h>     
#include <GLFW/glfw3.h>

#include <vector>

struct mesh {
    std::vector <vertex> vertices;
    std::vector <unsigned int> index;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    bool generated;

    mesh(std::vector<vertex>& v, std::vector<unsigned int>& i)
        : vertices(v), index(i), generated(false) {}
};

#endif
