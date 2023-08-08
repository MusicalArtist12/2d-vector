#pragma once

#include <glad/gl.h>     
#include <GLFW/glfw3.h>     

#include <vector>
#include <iostream>
#include <string>

struct vertex {
    float pos[2];
    float rgba[4];

    vertex() {}

    vertex(float x, float y, float r, float g, float b, float a) {
        pos[0] = x;
        pos[1] = y;

        rgba[0] = r;
        rgba[1] = g;
        rgba[2] = b;
        rgba[3] = a;
    }
};

struct mesh {
    std::vector <vertex> vertices;
    std::vector <GLuint> index;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    mesh(std::vector<vertex>& v, std::vector<GLuint>& i) {
        vertices = v;
        index = i;

        genBuffer();
    }

    void genBuffer();
};

void mesh::genBuffer() {
    glGenVertexArrays(1, &VAO); 

    glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, (vertices.size() * sizeof(vertex)), vertices.data(), GL_STATIC_DRAW);
        // type of buffer, size of buffer we want to pass, data, how its managed
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (index.size() * sizeof(GLuint)), index.data(), GL_STATIC_DRAW);

        // Position
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
        glEnableVertexAttribArray(0);

        // vertex rgba
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(2* sizeof(float)));
        glEnableVertexAttribArray(1);

    glBindVertexArray(0); 
}