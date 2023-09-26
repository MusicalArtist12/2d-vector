#pragma once

#include <glad/gl.h>     
#include <GLFW/glfw3.h>     

#include <vector>
#include <iostream>
#include <string>

struct mesh {
    std::vector <vertex> vertices;
    std::vector <GLuint> index;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    bool generated;

    mesh(std::vector<vertex>& v, std::vector<GLuint>& i)
        : vertices(v), index(i), generated(false) {}
};
