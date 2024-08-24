#include "Mesh.h"
#include "Shader.h"

#include <cmath>
#include <iostream>

Mesh::Mesh(std::vector<Vertex>& v, std::vector<unsigned int>& i): numVertices(v.size()), numIndices(i.size()) {
    Shader::generateBuffer(&VAO, &VBO, &EBO, v, i);
}

float Mesh::radius(glm::vec3 scale) {
    float radius = 0.0f;

    BufferMap vbo = BufferMap(&VAO, &VBO, GL_ARRAY_BUFFER, GL_READ_WRITE);
    Vertex* vertices = (Vertex*)vbo.data;

    // adjust each point to match the center, then find the distance
    for (int i = 0; i < numVertices; i++) {
        vertices[i].pos.x = vertices[i].pos.x - center(vertices).x;
        vertices[i].pos.y = vertices[i].pos.y - center(vertices).y;
    
        float distance = sqrt(pow(vertices[i].pos.x * scale.x, 2) + pow(vertices[i].pos.y * scale.y, 2));

        if (radius < distance) {
            radius = distance;
        }
    }

    return radius;
}

glm::vec3 Mesh::center(Vertex* vertices) {
    // find the center via the mean
    float x_mean = 0.0f;
    float y_mean = 0.0f;

    for (int i = 0; i < numVertices; i++) {
        x_mean += vertices[i].pos[0];
        y_mean += vertices[i].pos[1];
    }  

    x_mean = x_mean / numVertices;
    y_mean = y_mean / numVertices;

    return glm::vec3(x_mean, y_mean, 0.0f);
}

Mesh genPolygon(int numSides) {
    if (numSides < 3) {
        throw std::invalid_argument("genPolygon() cannot generate a polygon with less than 3 sides");
    }

    std::vector<Vertex> vertices = {
        Vertex(0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f)
    };

    std::vector<unsigned int> index;

    float angle = 360.0/(float)numSides;
    int idx_a = 1; // 1 -> numSides
    int idx_b = 2; // 2 -> numSides -> 1

    for (int i = 0; i < numSides; i++) {
        // essentially, there will be numsides amount of triangles all around the center

        float x = glm::cos(glm::radians(i * angle));
        float y = glm::sin(glm::radians(i * angle));

        vertices.push_back(Vertex(x, y, 1.0f, 1.0f, 1.0f, 1.0f));
        index.push_back(0); 
        
        index.push_back(idx_a);
        idx_a++;

        index.push_back(idx_b);
        idx_b = (idx_b + 1) % (numSides + 1);

        if (idx_b == 0) {
            idx_b = 1;
        }

    }
    
    return Mesh(vertices, index);
}

void setColor(Mesh* mesh, float r, float g, float b, float a) {
    BufferMap vbo = BufferMap(&mesh->VAO, &mesh->VBO, GL_ARRAY_BUFFER, GL_READ_WRITE);
    Vertex* vertices = (Vertex*)vbo.data;

    for (int i = 0; i < mesh->numVertices; i++) {
        vertices[i].rgba[0] = r;
        vertices[i].rgba[1] = g;
        vertices[i].rgba[2] = b;
        vertices[i].rgba[3] = a;
    }
}